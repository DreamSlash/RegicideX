// Fill out your copyright notice in the Description page of Project Settings.

#include "RGX_BTTask_EnemyStrafing.h"

#include "AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"

EBTNodeResult::Type URGX_BT_EnemyStrafing::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bNotifyTick = true;

	const AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* ControlledPawn = AIController->GetPawn();
	ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(ControlledPawn);
	Enemy->Orbiting = true;

	Direction = GetDirection(OwnerComp);

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type URGX_BT_EnemyStrafing::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* ControlledPawn = AIController->GetPawn();
	if (ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(ControlledPawn))
	{
		Enemy->Orbiting = false;
	}

	return EBTNodeResult::Aborted;
}

void URGX_BT_EnemyStrafing::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* ControlledPawn = AIController->GetPawn();
	ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(ControlledPawn);

	if (Enemy->TargetActor == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	ControlledPawn->AddMovementInput(Direction);

	FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
}

void URGX_BT_EnemyStrafing::InitializeFromAsset(UBehaviorTree& Asset)
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

FVector URGX_BT_EnemyStrafing::GetDirection(UBehaviorTreeComponent& OwnerComp) const
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	if (MyBlackboard)
	{
		if (DirectionKey.SelectedKeyType == UBlackboardKeyType_Int::StaticClass())
		{
			int32 KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Int>(DirectionKey.GetSelectedKeyID());
			const AAIController* AIController = OwnerComp.GetAIOwner();
			APawn* ControlledPawn = AIController->GetPawn();

			const FVector forwardVector = ControlledPawn->GetActorForwardVector();
			const FVector direction = forwardVector.RotateAngleAxis((float)KeyValue, ControlledPawn->GetActorUpVector());
			return direction;

			/*switch (KeyValue)
			{
			case 0:
				return ControlledPawn->GetActorRightVector();
			case 1:
				return -ControlledPawn->GetActorRightVector();
			case 2:
				return ControlledPawn->GetActorForwardVector();
			case 3:
				return -ControlledPawn->GetActorForwardVector();
			default:
				return FVector::ZeroVector;
			}*/
		}
	}

	return FVector::ZeroVector;
}
