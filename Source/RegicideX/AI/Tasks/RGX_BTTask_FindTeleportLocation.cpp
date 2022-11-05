// Fill out your copyright notice in the Description page of Project Settings.

#include "RGX_BTTask_FindTeleportLocation.h"

#include "AIController.h"

#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RegicideX/Actors/RGX_CombatManager.h"
#include "RegicideX/AI/Controllers/RGX_EnemyBaseController.h"

//#pragma optimize("", off)

URGX_BT_FindTeleportLocation::URGX_BT_FindTeleportLocation(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Find Teleport Location";
}

EBTNodeResult::Type URGX_BT_FindTeleportLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Controller = Cast<ARGX_EnemyBaseController>(OwnerComp.GetAIOwner());

	if (Controller && LocationSeekerQuery)
	{
		LocationSeekerQueryRequest = FEnvQueryRequest(LocationSeekerQuery, Controller->Agent);
		LocationSeekerQueryRequest.SetFloatParam("Donut.InnerRadius", TeleportDistance);
		LocationSeekerQueryRequest.Execute(EEnvQueryRunMode::AllMatching, this, &URGX_BT_FindTeleportLocation::LocationSeekerQueryFinished);
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void URGX_BT_FindTeleportLocation::LocationSeekerQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	if (Result->IsAborted())
	{
		return;
	}

	if (Result->Items.Num() > 0)
	{
		float currentBestScore = 0;
		TArray<FVector> locations;
		Result->GetAllAsLocations(locations);
		SetLocation(*Controller->BehaviorTreeComponent, Result->GetItemAsLocation(0));
		FinishLatentTask(*Controller->BehaviorTreeComponent, EBTNodeResult::Succeeded);
	}	
	else
	{
		FinishLatentTask(*Controller->BehaviorTreeComponent, EBTNodeResult::Failed);
	}
}

void URGX_BT_FindTeleportLocation::SetLocation(UBehaviorTreeComponent& OwnerComp, const FVector& location)
{
	if (UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent())
	{
		if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
		{
			MyBlackboard->SetValue<UBlackboardKeyType_Vector>(GetSelectedBlackboardKey(), location);
		}
	}
}


