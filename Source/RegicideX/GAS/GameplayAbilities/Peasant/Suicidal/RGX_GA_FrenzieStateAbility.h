// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RegicideX/GAS/GameplayAbilities/BaseAbilities/RGX_GA_CastSkillAbility.h"
#include "GameplayEffect.h"
#include "Materials/MaterialInstance.h"
#include "RGX_GA_FrenzieStateAbility.generated.h"

/**
 * 
 */
UCLASS()
class REGICIDEX_API URGX_GA_FrenzieStateAbility : public URGX_CastSkillAbility
{
	GENERATED_BODY()
	
public:
	URGX_GA_FrenzieStateAbility();

protected:
	bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	
	void OnReceivedEvent(FGameplayTag EventTag, FGameplayEventData EventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag FrenzieTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float FrenzieSpeedMultiplier = 2.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UMaterialInstance* FrenziedMaterial;
};
