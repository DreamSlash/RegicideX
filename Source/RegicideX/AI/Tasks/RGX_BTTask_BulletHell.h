// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RGX_BTTask_BulletHell.generated.h"

/**
 * 
 */
class ARGX_DistanceAngel;

UCLASS()
class REGICIDEX_API URGX_BTTask_BulletHell : public UBTTaskNode
{
	GENERATED_BODY()

	float TaskTime = 0.0;

	float RotationSpeedMultiplier = 2.0;

	ARGX_DistanceAngel* DistAngelPawn;;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxTime = 5.0;


	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	void Shoot();

};
