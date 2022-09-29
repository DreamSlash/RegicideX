// Fill out your copyright notice in the Description page of Project Settings.

#include "RGX_BTTask_FindStrafeDirection.h"

#include "AIController.h"

#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

#include "RegicideX/Actors/RGX_CombatManager.h"
#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RegicideX/AI/Controllers/RGX_EnemyBaseController.h"

//#pragma optimize("", off)

URGX_BT_FindStrafeDirection::URGX_BT_FindStrafeDirection(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Find Strafe Direction";
}

EBTNodeResult::Type URGX_BT_FindStrafeDirection::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Controller = Cast<ARGX_EnemyBaseController>(OwnerComp.GetAIOwner());

	if (Controller && LocationSeekerQuery)
	{
		const float playerDistance = GetDistance(OwnerComp);
		if (playerDistance > 0.0)
		{
			LocationSeekerQueryRequest = FEnvQueryRequest(LocationSeekerQuery, Controller->Agent);
			LocationSeekerQueryRequest.SetFloatParam("OnCircle.CircleRadius", playerDistance);
			LocationSeekerQueryRequest.Execute(EEnvQueryRunMode::AllMatching, this, &URGX_BT_FindStrafeDirection::LocationSeekerQueryFinished);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}

void URGX_BT_FindStrafeDirection::InitializeFromAsset(UBehaviorTree& Asset)
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

void URGX_BT_FindStrafeDirection::LocationSeekerQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	int32 index = 0;
	float currentBestScore = 0;
	TArray<FVector> locations;
	Result->GetAllAsLocations(locations);

	for (auto& loc : locations)
	{
		//DrawDebugCapsule(GetWorld(), loc, 200, 100, FQuat::Identity, FColor::Purple, false, 2);

		if (IsDistanceGreaterThanX(loc) && Result->GetItemScore(index) > currentBestScore)
		{
			StrafeLocation = loc;
			currentBestScore = Result->GetItemScore(index);
		}

		++index;
	}

	const FVector dirStrafe = (StrafeLocation - Controller->Agent->GetActorLocation()).GetSafeNormal2D();
	if (FVector::DotProduct(Controller->Agent->GetActorRightVector(), dirStrafe) >= 0.0)
	{
		Controller->BlackboardComponent->SetValue<UBlackboardKeyType_Enum>(Controller->StrafeDirectionKeyId, ERGX_StrafeDirection::Right);
	}
	else
	{
		Controller->BlackboardComponent->SetValue<UBlackboardKeyType_Enum>(Controller->StrafeDirectionKeyId, ERGX_StrafeDirection::Left);
	}
}

bool URGX_BT_FindStrafeDirection::IsDistanceGreaterThanX(const FVector& Location)
{
	for (const auto& item : Controller->CombatManager->EnemyMeleeItems)
	{
		if (item.IsValid())
		{
			const float distance = (Location - item.Enemy->GetActorLocation()).Size2D();
			if (distance <= Distance)
			{
				return false;
			}
		}
	}

	for (const auto& item : Controller->CombatManager->EnemyRangedItems)
	{
		if (item.IsValid())
		{
			const float distance = (Location - item.Enemy->GetActorLocation()).Size2D();
			if (distance <= Distance)
			{
				return false;
			}
		}
	}

	return true;
}

float URGX_BT_FindStrafeDirection::GetDistance(UBehaviorTreeComponent& OwnerComp) const
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
