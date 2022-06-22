// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RGX_BTTask_Levitate.generated.h"

/**
 * 
 */
class ARGX_MeleeAngel;

UCLASS()
class REGICIDEX_API URGX_BTTask_Levitate : public UBTTaskNode
{
	GENERATED_BODY()
	
	float TaskTime = 0.0;

	ARGX_MeleeAngel* MeleeAngelPawn = nullptr;

	FVector GoalLocation;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxTime = 5.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxLHeight = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LevitateSpeed = 600.0f;

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};






