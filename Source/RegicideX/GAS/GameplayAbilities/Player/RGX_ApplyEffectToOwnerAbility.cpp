#include "RGX_ApplyEffectToOwnerAbility.h"

#include "RegicideX/Actors/RGX_CharacterBase.h"

bool URGX_ApplyEffectToOwnerAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void URGX_ApplyEffectToOwnerAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const bool bActivationSuccessful = CommitAbility(Handle, ActorInfo, ActivationInfo);

	if (bActivationSuccessful == false)
		return;

	FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeOutgoingGameplayEffectSpec(EffectToApply, 1);

	FGameplayEffectContextHandle EffectContext = MakeEffectContext(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo());

	FGameplayEffectSpec* GESpec = GameplayEffectSpecHandle.Data.Get();
	GESpec->SetContext(EffectContext);

	AActor* OwnerActor = GetOwningActorFromActorInfo();
	ARGX_CharacterBase* OwnerCharacter = Cast<ARGX_CharacterBase>(OwnerActor);
	UAbilitySystemComponent* OwnerACS = OwnerCharacter->GetAbilitySystemComponent();
	OwnerACS->ApplyGameplayEffectSpecToSelf(*GESpec);

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void URGX_ApplyEffectToOwnerAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}