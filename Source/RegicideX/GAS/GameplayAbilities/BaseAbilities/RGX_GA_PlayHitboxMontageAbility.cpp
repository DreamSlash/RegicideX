// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_GA_PlayHitboxMontageAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RegicideX/Components/RGX_HitboxesManagerComponent.h"
#include "RegicideX/Components/RGX_CombatAssistComponent.h"
#include "RegicideX/Character/RGX_PlayerCharacter.h"
#include "RegicideX/Components/RGX_HitboxComponent.h"

URGX_PlayHitboxMontageAbility::URGX_PlayHitboxMontageAbility()
{
	//EventTagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Montage")));
}

bool URGX_PlayHitboxMontageAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	const bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (bResult == false)
		return false;

	const URGX_HitboxesManagerComponent* HitboxManagerComponent = ActorInfo->AvatarActor->FindComponentByClass<URGX_HitboxesManagerComponent>();
	if (HitboxManagerComponent == nullptr)
		return false;

	return true;
}

void URGX_PlayHitboxMontageAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData); // Parent already calls CommitAbility function

	// Play a montage that will trigger OnReceivedEvent callback if a GameplayEvent.Montage tag is called to be handled.
	PlayMontageAndWaitForEventTask = URGX_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
		this, NAME_None, MontageToPlay, EventTagContainer, PlayRate, StartSectionName, true);
	PlayMontageAndWaitForEventTask->OnInterrupted.AddDynamic(this, &URGX_PlayHitboxMontageAbility::OnMontageFinished);
	PlayMontageAndWaitForEventTask->OnBlendOut.AddDynamic(this, &URGX_PlayHitboxMontageAbility::OnMontageFinished);
	PlayMontageAndWaitForEventTask->OnCancelled.AddDynamic(this, &URGX_PlayHitboxMontageAbility::OnMontageFinished);
	PlayMontageAndWaitForEventTask->OnCompleted.AddDynamic(this, &URGX_PlayHitboxMontageAbility::OnMontageFinished);
	PlayMontageAndWaitForEventTask->EventReceived.AddDynamic(this, &URGX_PlayHitboxMontageAbility::OnReceivedEvent);
	PlayMontageAndWaitForEventTask->ReadyForActivation();
}

void URGX_PlayHitboxMontageAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URGX_PlayHitboxMontageAbility::OnMontageFinished(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void URGX_PlayHitboxMontageAbility::OnReceivedEvent(FGameplayTag EventTag, FGameplayEventData EventData)
{
	AActor* OwnerActor = GetOwningActorFromActorInfo();
	ARGX_CharacterBase* OwnerCharacter = Cast<ARGX_CharacterBase>(OwnerActor);
	UAbilitySystemComponent* OwnerACS = OwnerCharacter->GetAbilitySystemComponent();
	const ARGX_CharacterBase* TargetCharacter = Cast<ARGX_CharacterBase>(EventData.Target);
	UAbilitySystemComponent* TargetACS = TargetCharacter ? TargetCharacter->GetAbilitySystemComponent() : nullptr;

	// Find the effect mapped to the triggering event tag to apply to target.
	if (EffectToApplyToTargetWithPayload.Contains(EventTag) && TargetACS)
	{
		TSubclassOf<UGameplayEffect> GameplayEffectToApply = EffectToApplyToTargetWithPayload.Find(EventTag)->EffectToApply;
		FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeOutgoingGameplayEffectSpec(GameplayEffectToApply, OwnerCharacter->GetCharacterLevel());

		FGameplayEffectContextHandle EffectContext = MakeEffectContext(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo());
		FRGX_GameplayEffectContext* GameplayEffectContext = static_cast<FRGX_GameplayEffectContext*>(EffectContext.Get());
		GameplayEffectContext->OptionalObject = EffectToApplyToTargetWithPayload.Find(EventTag)->Payload;

		FGameplayEffectSpec* GESpec = GameplayEffectSpecHandle.Data.Get();
		GESpec->SetContext(EffectContext);
		TargetACS->ApplyGameplayEffectSpecToSelf(*GESpec);
	}

	// Apply own effects, such as cooldowns.
	if (EffectToApplyToOwnerWithPayload.Contains(EventTag) && OwnerACS)
	{
		TSubclassOf<UGameplayEffect> GameplayEffectToApply = EffectToApplyToOwnerWithPayload.Find(EventTag)->EffectToApply;
		FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeOutgoingGameplayEffectSpec(GameplayEffectToApply, OwnerCharacter->GetCharacterLevel());

		FGameplayEffectContextHandle EffectContext = MakeEffectContext(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo());
		FRGX_GameplayEffectContext* GameplayEffectContext = static_cast<FRGX_GameplayEffectContext*>(EffectContext.Get());
		GameplayEffectContext->OptionalObject = EffectToApplyToOwnerWithPayload.Find(EventTag)->Payload;

		FGameplayEffectSpec* GESpec = GameplayEffectSpecHandle.Data.Get();
		GESpec->SetContext(EffectContext);
		OwnerACS->ApplyGameplayEffectSpecToSelf(*GESpec);
	}
}

void URGX_PlayHitboxMontageAbility::PopulateGameplayEffectContext(FRGX_GameplayEffectContext& GameplayEffectContext)
{
}
