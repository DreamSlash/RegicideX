// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/MCV_GameplayAbility.h"
#include "RGX_TeleportAbility.generated.h"

/**
 * 
 */


UCLASS()
class REGICIDEX_API URGX_TeleportAbility : public UMCV_GameplayAbility
{
	GENERATED_BODY()

		URGX_TeleportAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

public:
	UPROPERTY(EditAnywhere)
		FVector LocationToTeleport;

};
