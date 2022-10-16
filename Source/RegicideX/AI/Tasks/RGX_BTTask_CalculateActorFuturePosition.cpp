// Fill out your copyright notice in the Description page of Project Settings.

#include "RGX_BTTask_CalculateActorFuturePosition.h"

#include "AIController.h"

#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"

EBTNodeResult::Type URGX_BTTask_CalculateActorFuturePosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const bool bTargetKeyIsValid = OwnerComp.GetBlackboardComponent()->IsValidKey(OwnerComp.GetBlackboardComponent()->GetKeyID(TargetKey.SelectedKeyName));
	const bool bPositionKeyIsValid = OwnerComp.GetBlackboardComponent()->IsValidKey(OwnerComp.GetBlackboardComponent()->GetKeyID(PositionKey.SelectedKeyName));
	if (bTargetKeyIsValid == false || bPositionKeyIsValid == false)
	{
		return EBTNodeResult::Failed;
	}

	const AActor* targetActor = GetTargetActor(OwnerComp);
	if (targetActor == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	const FVector futurePosition = CalculateFuturePosition(OwnerComp, targetActor);
	if (futurePosition == FVector::ZeroVector)
	{
		return EBTNodeResult::Failed;
	}

	if (SetPositionValue(OwnerComp, futurePosition) == false)
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}

void URGX_BTTask_CalculateActorFuturePosition::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (BBAsset)
	{
		TargetKey.ResolveSelectedKey(*BBAsset);
		PositionKey.ResolveSelectedKey(*BBAsset);
	}
	else
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("Can't initialize task: %s, make sure that behavior tree specifies blackboard asset!"), *GetName());
	}
}

const AActor* URGX_BTTask_CalculateActorFuturePosition::GetTargetActor(UBehaviorTreeComponent& OwnerComp) const
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	if (MyBlackboard)
	{
		if (TargetKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
		{
			UObject* KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Object>(TargetKey.GetSelectedKeyID());
			return Cast<AActor>(KeyValue);
		}
	}

	return nullptr;
}

FVector URGX_BTTask_CalculateActorFuturePosition::CalculateFuturePosition(UBehaviorTreeComponent& OwnerComp, const AActor* Target) const
{
	const AAIController* aiController = OwnerComp.GetAIOwner();

	ARGX_EnemyBase* owner = Cast<ARGX_EnemyBase>(aiController->GetPawn());
	if (owner)
	{
		const float distance = FVector::Distance(owner->GetActorLocation(), Target->GetActorLocation());
		const float timeToReachTarget = distance / owner->GetCurrentMaxSpeed();

		const FVector targetVelocity = Target->GetVelocity();
		const FVector futureTargetPosition = targetVelocity* timeToReachTarget;
		return futureTargetPosition;
	}

	return FVector::ZeroVector;
}

bool URGX_BTTask_CalculateActorFuturePosition::SetPositionValue(UBehaviorTreeComponent& OwnerComp, const FVector& Position)
{
	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	if (MyBlackboard)
	{
		if (PositionKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
		{
			return MyBlackboard->SetValue<UBlackboardKeyType_Vector>(PositionKey.GetSelectedKeyID(), Position);
		}
	}

	return false;
}


