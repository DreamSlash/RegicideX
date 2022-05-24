// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RGX_BTTask_MeleeAngelCharge.generated.h"

/**
 * 
 */
class ARGX_MeleeAngel;

UCLASS()
class REGICIDEX_API URGX_BTTask_MeleeAngelCharge : public UBTTaskNode
{
	GENERATED_BODY()
	
	float TaskTime = 0.0;

	float AccelerationMultiplier = 200.0; float MoveSpeed = 100.0f;

	ARGX_MeleeAngel* MeleeAngelPawn = nullptr;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxTime = 5.0;

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
