// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "RegicideX/GAS/GameplayAbilities/BaseAbilities/RGX_GA_CastSkillAbility.h"
#include "RGX_BurstBullet.generated.h"

class ARGX_Projectile;

UCLASS()
class REGICIDEX_API URGX_BurstBullet : public URGX_CastSkillAbility
{
	GENERATED_BODY()
	
private:
	int Counter = 0; // Number of bullets shoot
	ACharacter* Character = nullptr;

public:
	UPROPERTY(EditDefaultsOnly)
		int NumberProjectilesToFire = 5;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARGX_Projectile> BulletBP;


protected:
	void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void OnReceivedEvent(
		FGameplayTag EventTag,
		FGameplayEventData EventData);

	void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled) override;

	void SpawnBullet();

	UPROPERTY()
		FGenericTeamId TeamIdToApply;

	FTransform Transform;
};
