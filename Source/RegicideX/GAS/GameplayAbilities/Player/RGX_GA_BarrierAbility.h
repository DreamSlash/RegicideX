#pragma once

#include "CoreMinimal.h"
#include "RegicideX/GAS/GameplayAbilities/BaseAbilities/RGX_GA_CastSkillAbility.h"
#include "RegicideX/GAS/RGX_PayloadObjects.h"
#include "RGX_GA_BarrierAbility.generated.h"

UCLASS()
class REGICIDEX_API URGX_BarrierAbility : public URGX_CastSkillAbility
{
	GENERATED_BODY()

public:
	URGX_BarrierAbility();

protected:
	bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const override;

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	void OnReceivedEvent(FGameplayTag EventTag, FGameplayEventData EventData) override;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> BarrierClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	URGX_LaunchEventDataAsset* LaunchEventData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 200.0f;
};