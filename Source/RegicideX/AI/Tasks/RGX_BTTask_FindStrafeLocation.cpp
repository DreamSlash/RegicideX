// Fill out your copyright notice in the Description page of Project Settings.

#include "RGX_BTTask_FindStrafeLocation.h"

#include "AIController.h"

#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RegicideX/Actors/RGX_CombatManager.h"
#include "RegicideX/AI/Controllers/RGX_EnemyBaseController.h"

//#pragma optimize("", off)

URGX_BT_FindStrafeLocation::URGX_BT_FindStrafeLocation(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Find Strafe Location";
}

EBTNodeResult::Type URGX_BT_FindStrafeLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Controller = Cast<ARGX_EnemyBaseController>(OwnerComp.GetAIOwner());

	if (Controller && LocationSeekerQuery)
	{
		const float playerDistance = GetDistance(OwnerComp);
		if (playerDistance > 0.0)
		{
			LocationSeekerQueryRequest = FEnvQueryRequest(LocationSeekerQuery, Controller->Agent);
			LocationSeekerQueryRequest.SetFloatParam("OnCircle.CircleRadius", playerDistance);
			LocationSeekerQueryRequest.SetFloatParam("Distance.FloatValueMax", MaxDistanceFromOwner);
			LocationSeekerQueryRequest.Execute(EEnvQueryRunMode::AllMatching, this, &URGX_BT_FindStrafeLocation::LocationSeekerQueryFinished);
			return EBTNodeResult::InProgress;
		}
	}

	return EBTNodeResult::Failed;
}

void URGX_BT_FindStrafeLocation::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (BBAsset)
	{
		DistanceKey.ResolveSelectedKey(*BBAsset);
	}
	else
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("Can't initialize task: %s, make sure that behavior tree specifies blackboard asset!"), *GetName());
	}
}

void URGX_BT_FindStrafeLocation::LocationSeekerQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	float currentBestScore = 0;
	TArray<FVector> locations;
	Result->GetAllAsLocations(locations);

	TArray<TTuple<FVector, float>> locationsRight;
	TArray<TTuple<FVector, float>> locationsLeft;
	const FVector fromLocation = Controller->Agent->GetActorLocation();
	const FVector rightVector = Controller->Agent->GetActorRightVector();
	
	int32 index = 0;
	for (auto& loc : locations)
	{
		/*if (FVector::Dist2D(fromLocation, loc) >= Distance)
		{*/
			//DrawDebugCapsule(GetWorld(), loc, 200, 100, FQuat::Identity, FColor::Purple, false, 2);

			const FVector direction = (loc - fromLocation).GetSafeNormal2D();
			if (FVector::DotProduct(rightVector, direction) >= 0.0)
			{
				locationsRight.Add(MakeTuple(loc, Result->GetItemScore(index++)));
			}
			else
			{
				locationsLeft.Add(MakeTuple(loc, Result->GetItemScore(index++)));
			}
		/*}
		else
		{
			++index;
		}*/
	}

	auto sortFunction = [fromLocation](const auto& left, const auto& right)
	{
		const float distanceLeft = FVector::Dist2D(fromLocation, left.Key);
		const float distanceRight = FVector::Dist2D(fromLocation, right.Key);
		return distanceLeft < distanceRight;
	};

	locationsRight.Sort(sortFunction);
	locationsLeft.Sort(sortFunction);

	int32 bestLocationRightIndex = BestReachableLocationInDirection(locationsRight);
	int32 bestLocationLeftIndex = BestReachableLocationInDirection(locationsLeft);

	UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(GetOuter());
	if (bestLocationRightIndex < 0 && bestLocationLeftIndex < 0)
	{
		FinishLatentTask(*OwnerComp, EBTNodeResult::Failed);
	}
	else if (bestLocationRightIndex < 0)
	{
		const auto& bestLeft = locationsLeft[bestLocationLeftIndex];
		Controller->BlackboardComponent->SetValue<UBlackboardKeyType_Vector>(Controller->StrafeLocationKeyId, bestLeft.Key);
		FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
	}
	else if (bestLocationLeftIndex < 0)
	{
		const auto& bestRight = locationsRight[bestLocationRightIndex];
		Controller->BlackboardComponent->SetValue<UBlackboardKeyType_Vector>(Controller->StrafeLocationKeyId, bestRight.Key);
		FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		const auto& bestLeft = locationsLeft[bestLocationLeftIndex];
		const auto& bestRight = locationsRight[bestLocationRightIndex];

		FVector finalStrafeLocation;
		if (bestRight.Value >= bestLeft.Value)
		{
			finalStrafeLocation = bestRight.Key;
		}
		else
		{
			finalStrafeLocation = bestLeft.Key;
		}

		Controller->BlackboardComponent->SetValue<UBlackboardKeyType_Vector>(Controller->StrafeLocationKeyId, finalStrafeLocation);
		FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
	}
}

int32 URGX_BT_FindStrafeLocation::BestReachableLocationInDirection(const TArray<TTuple<FVector, float>>& Locations) const
{
	int32 currentBestIndex = -1;
	float currentBestScore = 0.0;

	int32 index = 0;
	for (const auto& loc : Locations)
	{
		const FVector& location = loc.Key;
		float score = loc.Value;

		if (IsDistanceGreaterThanX(location) && score > currentBestScore)
		{
			currentBestIndex = index;
			currentBestScore = score;
		}
		else
		{
			break;
		}

		++index;
	}

	return currentBestIndex;
}

bool URGX_BT_FindStrafeLocation::IsDistanceGreaterThanX(const FVector& Location) const
{
	for (const auto& item : Controller->CombatManager->EnemyMeleeItems)
	{
		if (item.IsValid() && item.Enemy != Controller->Agent)
		{
			const float distance = (Location - item.Enemy->GetActorLocation()).Size2D();
			if (distance <= MinDistanceToOtherEnemies)
			{
				return false;
			}
		}
	}

	for (const auto& item : Controller->CombatManager->EnemyRangedItems)
	{
		if (item.IsValid() && item.Enemy != Controller->Agent)
		{
			const float distance = (Location - item.Enemy->GetActorLocation()).Size2D();
			if (distance <= MinDistanceToOtherEnemies)
			{
				return false;
			}
		}
	}

	return true;
}

float URGX_BT_FindStrafeLocation::GetDistance(UBehaviorTreeComponent& OwnerComp) const
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	if (MyBlackboard)
	{
		if (DistanceKey.SelectedKeyType == UBlackboardKeyType_Float::StaticClass())
		{
			return MyBlackboard->GetValue<UBlackboardKeyType_Float>(DistanceKey.GetSelectedKeyID());
		}
	}

	return 0.0f;
}

