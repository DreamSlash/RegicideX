
#pragma once

#include "CoreMinimal.h"
#include "RegicideX/GAS/GameplayAbilities/Common/RGX_GA_MeleeAttackAbility.h"
#include "RGX_DivineDescent.generated.h"

class ARGX_MeleeAngel;

UCLASS()
class REGICIDEX_API URGX_DivineDescent : public URGX_MeleeAttackAbility, public FTickableGameObject
{
	GENERATED_BODY()

protected:
	bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayTagContainer* SourceTags = nullptr, 
		const FGameplayTagContainer* TargetTags = nullptr, 
		OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, 
		const FGameplayEventData* TriggerEventData) override;

	void EndAbility(
		const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, 
		bool bReplicateEndAbility, 
		bool bWasCancelled) override;

	void Tick(float DeltaTime) override;

	TStatId GetStatId() const override;

private:

	bool bFinished = false;
	bool bTargetSelected = false;
	bool bAdditionalTranslationEnded = false;
	FVector StartLocation = FVector::ZeroVector;
	FVector TargetLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	float Speed = 1000.0f;
};
