// Fill out your copyright notice in the Description page of Project Settings.

#include "RGX_BTTask_FindStrafeLocation.h"

#include "AIController.h"

#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RegicideX/AI/Controllers/RGX_EnemyBaseController.h"

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
		LocationSeekerQueryRequest = FEnvQueryRequest(LocationSeekerQuery, Controller->Agent);
		LocationSeekerQueryRequest.Execute(EEnvQueryRunMode::AllMatching, this, &URGX_BT_FindStrafeLocation::LocationSeekerQueryFinished);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

void URGX_BT_FindStrafeLocation::LocationSeekerQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	int32 index = 0;
	float currentBestScore = 0;
	TArray<FVector> locations;
	Result->GetAllAsLocations(locations);

	for (auto& loc : locations)
	{
		DrawDebugCapsule(GetWorld(), loc, 200, 100, FQuat::Identity, FColor::Purple, false, 2);

		if (IsDistanceGreaterThanX(loc) && Result->GetItemScore(index) > currentBestScore)
		{
			StrafeLocation = loc;
			currentBestScore = Result->GetItemScore(index);
		}

		++index;
	}

	//Controller->BlackboardComponent->SetValue<UBlackboardKeyType_Vector>(Controller->StrafeToLocationKeyId, StrafeLocation);
}

bool URGX_BT_FindStrafeLocation::IsDistanceGreaterThanX(const FVector& Location)
{
	return true;
}
