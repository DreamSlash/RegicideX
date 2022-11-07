// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RGX_Peasant.h"
#include "RGX_Peasant_Shield.generated.h"


UCLASS()
class REGICIDEX_API ARGX_Peasant_Shield : public ARGX_Peasant
{

	GENERATED_BODY()

public:
	ARGX_Peasant_Shield();

	UPROPERTY(EditDefaultsOnly)
		UAnimMontage* AMShieldBlock = nullptr;

	UPROPERTY(EditDefaultsOnly)
		UAnimMontage* AMShieldBlockBreaks = nullptr;

	UPROPERTY(EditDefaultsOnly)
		UAnimMontage* AMShieldBreaks = nullptr;

	UPROPERTY(EditDefaultsOnly)
		float ShieldAmount = 100.0f;

	UFUNCTION(BlueprintImplementableEvent)
	void OnShieldCracked();

	UFUNCTION(BlueprintImplementableEvent)
		void OnShieldDestroyed();

protected:
	virtual bool CanBeLaunched(AActor* ActorInstigator, URGX_LaunchEventDataAsset* LaunchPayload) override;

	virtual float HandleDamageMitigation(
		float DamageAmount,
		const FHitResult& HitInfo,
		const struct FGameplayTagContainer& DamageTags,
		ARGX_CharacterBase* InstigatorCharacter,
		AActor* DamageCauser) override;
};
