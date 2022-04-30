// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RGX_BTTask_ActivateAbility.generated.h"

/**
 * 
 */
class UGameplayAbility;

UCLASS()
class REGICIDEX_API URGX_BTTask_ActivateAbility : public UBTTaskNode
{
	GENERATED_BODY()

	URGX_BTTask_ActivateAbility();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:



	UPROPERTY(EditAnywhere)
		TSubclassOf<UGameplayAbility> AbilityToActivte;

	UFUNCTION()
		void OnAbilityEnded(UGameplayAbility* EndedAbility);
	
};
