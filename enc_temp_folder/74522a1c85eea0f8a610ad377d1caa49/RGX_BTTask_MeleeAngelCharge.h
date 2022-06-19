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

	float MoveSpeed = 2.0f;
	ARGX_MeleeAngel* MeleeAngelPawn = nullptr;
	FVector GoalLocation;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxTime = 5.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AccelerationMultiplier = 1.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bDebug = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayTag HitboxTag;

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
