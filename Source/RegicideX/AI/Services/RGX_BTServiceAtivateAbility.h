// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "RGX_BTServiceAtivateAbility.generated.h"

/**
 * 
 */
class UGameplayAbility;

UCLASS()
class REGICIDEX_API URGX_BTServiceAtivateAbility : public UBTService
{
	GENERATED_BODY()
	URGX_BTServiceAtivateAbility();

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayAbility> AbilityToActivte;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
