// Fill out your copyright notice in the Description page of Project Settings.

#include "RGX_BTTask_EnemyOrbit.h"

#include "AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"

EBTNodeResult::Type URGX_BT_EnemyOrbit::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bNotifyTick = true;

	const AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* ControlledPawn = AIController->GetPawn();
	ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(ControlledPawn);
	Enemy->Orbiting = true;

	KeyValue = GetKeyValue(OwnerComp);

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type URGX_BT_EnemyOrbit::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* ControlledPawn = AIController->GetPawn();
	if (ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(ControlledPawn))
	{
		Enemy->Orbiting = false;
	}

	return EBTNodeResult::Aborted;
}

void URGX_BT_EnemyOrbit::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* ControlledPawn = AIController->GetPawn();
	ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(ControlledPawn);

	if (Enemy->TargetActor == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	FVector direction;
	switch (KeyValue)
	{
	case ERGX_StrafeDirection::Right: // Right
		direction = ControlledPawn->GetActorRightVector();
		break;
	case ERGX_StrafeDirection::Left:
		direction = -ControlledPawn->GetActorRightVector();
		break;
	default:
		direction = FVector::ZeroVector;
	}

	ControlledPawn->AddMovementInput(direction);

	FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
}

void URGX_BT_EnemyOrbit::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (BBAsset)
	{
		DirectionKey.ResolveSelectedKey(*BBAsset);
	}
	else
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("Can't initialize task: %s, make sure that behavior tree specifies blackboard asset!"), *GetName());
	}
}

ERGX_StrafeDirection::Type URGX_BT_EnemyOrbit::GetKeyValue(UBehaviorTreeComponent& OwnerComp) const
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	if (MyBlackboard)
	{
		if (DirectionKey.SelectedKeyType == UBlackboardKeyType_Enum::StaticClass())
		{
			return (ERGX_StrafeDirection::Type)MyBlackboard->GetValue<UBlackboardKeyType_Enum>(DirectionKey.GetSelectedKeyID());
		}
	}

	return ERGX_StrafeDirection::None;
}
