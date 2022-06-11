// Fill out your copyright notice in the Description page of Project Settings.
#include "RGX_BTTask_DADamaged.h"

#include "RegicideX/Actors/Enemies/RGX_DistanceAngel.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type URGX_BTTask_DADamaged::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* ControlledPawn = AIController->GetPawn();

	DistAngelPawn = Cast<ARGX_DistanceAngel>(ControlledPawn);

	FVector NewPosition = DistAngelPawn->GetActorLocation() - DistAngelPawn->GetActorForwardVector() * RetreatAmount;

	DistAngelPawn->SetActorLocation(NewPosition);

	DistAngelPawn->ChangeEyeColor(FLinearColor(1.0, 0.0, 0.0, 1.0));

	bNotifyTick = true;
	return EBTNodeResult::InProgress;
}

void URGX_BTTask_DADamaged::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	TaskTime += DeltaSeconds;
	if (TaskTime >= MaxTime)
	{
		TaskTime = 0.0f;
		DistAngelPawn->ChangeEyeColor(FLinearColor(0.962998, 1.0, 0.742727, 1.0));
		AAIController* AIController = OwnerComp.GetAIOwner();
		UBlackboardComponent* BB = AIController->GetBlackboardComponent();
		BB->SetValueAsBool(TEXT("Attacked"), false);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}







