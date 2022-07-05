// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "RegicideX/GAS/GameplayAbilities/BaseAbilities/RGX_GA_CastSkillAbility.h"
#include "RGX_GA_ClusteredProjectiles.generated.h"

class ARGX_Projectile;

UCLASS()
class REGICIDEX_API URGX_GA_ClusteredProjectiles : public URGX_CastSkillAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
		int NumberProjectilesToFire = 15;

	//UPROPERTY(EditDefaultsOnly)
		TArray<FVector> PointsToSpawn;

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

	UPROPERTY()
	FGenericTeamId TeamIdToApply;

	FTransform Transform;
};
