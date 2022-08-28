
#include "RGX_DivineDescent.h"

#include "RegicideX/Actors/Enemies/RGX_MeleeAngel.h"

bool URGX_DivineDescent::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if(bResult == false)
		return false;

	return true;
}

void URGX_DivineDescent::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

}

void URGX_DivineDescent::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URGX_DivineDescent::Tick(float DeltaTime)
{
	if (!CurrentActorInfo)
		return;
	ARGX_MeleeAngel* owner = Cast<ARGX_MeleeAngel>(GetAvatarActorFromActorInfo());
	if (owner == nullptr || owner->bCharging == false) {
		UE_LOG(LogTemp, Warning, TEXT("Failing on charging!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("CHAAAAAAAAAAAAARGE!"));
}

TStatId URGX_DivineDescent::GetStatId() const
{
	return TStatId();
}
