// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_BTService_CalculateActorFuturePosition.h"

#include "AIController.h"

#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "DrawDebugHelpers.h"

#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"

//#pragma optimize("", off)

void URGX_BTService_CalculateActorFuturePosition::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const bool bTargetKeyIsValid = OwnerComp.GetBlackboardComponent()->IsValidKey(OwnerComp.GetBlackboardComponent()->GetKeyID(TargetKey.SelectedKeyName));
	const bool bPositionKeyIsValid = OwnerComp.GetBlackboardComponent()->IsValidKey(OwnerComp.GetBlackboardComponent()->GetKeyID(PositionKey.SelectedKeyName));
	if (bTargetKeyIsValid == false || bPositionKeyIsValid == false)
	{
		return;
	}

	const AActor* targetActor = GetTargetActor(OwnerComp);
	if (targetActor == nullptr)
	{
		return;
	}

	const FVector futurePosition = CalculateFuturePosition(OwnerComp, targetActor);
	if (futurePosition == FVector::ZeroVector)
	{
		return;
	}

	SetPositionValue(OwnerComp, futurePosition);
	//DrawDebugCapsule(GetWorld(), futurePosition, 200, 100, FQuat::Identity, FColor::Purple, false, 1);	
}

void URGX_BTService_CalculateActorFuturePosition::InitializeFromAsset(UBehaviorTree& Asset)
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

const AActor* URGX_BTService_CalculateActorFuturePosition::GetTargetActor(UBehaviorTreeComponent& OwnerComp) const
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

FVector URGX_BTService_CalculateActorFuturePosition::CalculateFuturePosition(UBehaviorTreeComponent& OwnerComp, const AActor* Target) const
{
	const AAIController* aiController = OwnerComp.GetAIOwner();

	ARGX_EnemyBase* owner = Cast<ARGX_EnemyBase>(aiController->GetPawn());
	if (owner)
	{
		const float distance = FVector::Distance(owner->GetActorLocation(), Target->GetActorLocation());
		const float timeToReachTarget = distance / owner->GetCurrentMaxSpeed();
		const float time = std::min(timeToReachTarget, MaxReachTime);
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("timeToReachTarget %f"), timeToReachTarget));

		const FVector targetVelocity = Target->GetVelocity();
		const FVector futureTargetPosition = Target->GetActorLocation() + (targetVelocity * time);
		return futureTargetPosition;
	}

	return FVector::ZeroVector;
}

bool URGX_BTService_CalculateActorFuturePosition::SetPositionValue(UBehaviorTreeComponent& OwnerComp, const FVector& Position)
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