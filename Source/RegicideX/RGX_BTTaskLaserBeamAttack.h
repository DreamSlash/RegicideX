// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RGX_BTTaskLaserBeamAttack.generated.h"

/**
 * 
 */
class ARGX_LaserBeamWeapon;

UCLASS()
class REGICIDEX_API URGX_BTTaskLaserBeamAttack : public UBTTaskNode
{
	GENERATED_BODY()

	float TaskTime = 0.0;

	ARGX_LaserBeamWeapon* LaserWeapon;

	AActor* OwnerActor;
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ARGX_LaserBeamWeapon> LaserBeamClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxTime = 5.0;

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
