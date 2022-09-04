// Fill out your copyright notice in the Description page of Project Settings.

#include "RGX_MageAngelController.h"

#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "BehaviorTree/BlackboardComponent.h"

void ARGX_MageAngelController::BeginPlay()
{
	Super::BeginPlay();
}

void ARGX_MageAngelController::DamageTaken()
{
	if (UBlackboardComponent* blackboard = GetBlackboardComponent())
	{
		const FName key = "ConsecutiveHits";
		int value = blackboard->GetValue<UBlackboardKeyType_Int>(key);
		blackboard->SetValue<UBlackboardKeyType_Int>(key, ++value);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [blackboard]() {
			const FName key = "ConsecutiveHits";
			blackboard->SetValue<UBlackboardKeyType_Int>(key, 0);
		}, TimeConsecutiveHits, false);
	}
}