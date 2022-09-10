// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_BTService_CalculateDistanceToActor.h"

#include "AIController.h"

#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "DrawDebugHelpers.h"

#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"

//#pragma optimize("", off)

void URGX_BTService_CalculateDistanceToActor::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const bool bTargetKeyIsValid = OwnerComp.GetBlackboardComponent()->IsValidKey(OwnerComp.GetBlackboardComponent()->GetKeyID(TargetKey.SelectedKeyName));
	const bool bBlackboardKeyIsValid = OwnerComp.GetBlackboardComponent()->IsValidKey(OwnerComp.GetBlackboardComponent()->GetKeyID(BlackboardKey.SelectedKeyName));
	if (bTargetKeyIsValid == false || bBlackboardKeyIsValid == false)
	{
		return;
	}

	const AActor* targetActor = GetTargetActor(OwnerComp);
	if (targetActor == nullptr)
	{
		return;
	}

	const float distance = CalculateDistanceBetweenActors(OwnerComp.GetAIOwner()->GetPawn(), targetActor);
	SetDistanceInBlackboard(OwnerComp, distance);
}

void URGX_BTService_CalculateDistanceToActor::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (BBAsset)
	{
		TargetKey.ResolveSelectedKey(*BBAsset);
		BlackboardKey.ResolveSelectedKey(*BBAsset);
	}
	else
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("Can't initialize task: %s, make sure that behavior tree specifies blackboard asset!"), *GetName());
	}
}

const AActor* URGX_BTService_CalculateDistanceToActor::GetTargetActor(UBehaviorTreeComponent& OwnerComp) const
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

float URGX_BTService_CalculateDistanceToActor::CalculateDistanceBetweenActors(const AActor* Owner, const AActor* Target) const
{
	const FVector ownerLocation = Owner->GetActorLocation();
	const FVector targetLocation = Target->GetActorLocation();

	DrawDebugCapsule(GetWorld(), ownerLocation, 200, 100, FQuat::Identity, FColor::Blue, false, 1);
	DrawDebugCapsule(GetWorld(), targetLocation, 200, 100, FQuat::Identity, FColor::Red, false, 1);

	return FVector::Dist2D(ownerLocation, targetLocation);
}

bool URGX_BTService_CalculateDistanceToActor::SetDistanceInBlackboard(UBehaviorTreeComponent& OwnerComp, const float Distance)
{
	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	if (MyBlackboard)
	{
		if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Float::StaticClass())
		{
			return MyBlackboard->SetValue<UBlackboardKeyType_Float>(BlackboardKey.GetSelectedKeyID(), Distance);
		}
	}

	return false;
}