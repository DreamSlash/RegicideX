// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_GA_MeleeAttackAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "RegicideX/Components/RGX_HitboxesManagerComponent.h"
#include "RegicideX/Components/RGX_CombatAssistComponent.h"
#include "RegicideX/Character/RGX_PlayerCharacter.h"
#include "RegicideX/Components/RGX_HitboxComponent.h"

URGX_MeleeAttackAbility::URGX_MeleeAttackAbility()
{
}

void URGX_MeleeAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	URGX_CombatAssistComponent* CombatAssistComponent = ActorInfo->AvatarActor->FindComponentByClass<URGX_CombatAssistComponent>(); 
	ARGX_PlayerCharacter* PlayerCharacter = Cast<ARGX_PlayerCharacter>(ActorInfo->AvatarActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->DisableMovementInput();

		if (CombatAssistComponent)
		{
			CombatAssistComponent->PerformAttackAutoAssist();
		}
	}

	if (CombatAssistComponent)
	{
		CombatAssistComponent->AddMovementVector(ActorInfo->AvatarActor->GetActorForwardVector(), MoveVectorLength, true);
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void URGX_MeleeAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ARGX_PlayerCharacter* PlayerCharacter = Cast<ARGX_PlayerCharacter>(CurrentActorInfo->AvatarActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->EnableMovementInput();
	}

	URGX_CombatAssistComponent* CombatAssistComponent = CurrentActorInfo->AvatarActor->FindComponentByClass<URGX_CombatAssistComponent>();
	if (CombatAssistComponent)
	{
		CombatAssistComponent->RemoveMovementVector();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URGX_MeleeAttackAbility::PopulateGameplayEffectContext(FRGX_GameplayEffectContext& GameplayEffectContext)
{
	float AbilityLevel = 1;

	FString ContextString;
	FRealCurve* DamageCurve = DamageLevelCurve->FindCurve(DamageCurveName, ContextString);
	FRealCurve* ScalingCurve = DamageLevelCurve->FindCurve(AttributeScalingCurveName, ContextString);
	GameplayEffectContext.DamageAmount = DamageCurve->Eval(AbilityLevel);
	GameplayEffectContext.ScalingAttributeFactor = ScalingCurve->Eval(AbilityLevel);
}

void URGX_MeleeAttackAbility::OnReceivedEvent(FGameplayTag EventTag, FGameplayEventData EventData)
{
	AActor* OwnerActor = GetOwningActorFromActorInfo();
	ARGX_CharacterBase* OwnerCharacter = Cast<ARGX_CharacterBase>(OwnerActor);
	const ARGX_CharacterBase* TargetCharacter = Cast<ARGX_CharacterBase>(EventData.Target);
	UAbilitySystemComponent* TargetACS = TargetCharacter->GetAbilitySystemComponent();

	// Find the effect mapped to the triggering event tag.
	if (EffectToApplyToTarget.Contains(EventTag))
	{
		TSubclassOf<UGameplayEffect>* GameplayEffectToApply = EffectToApplyToTarget.Find(EventTag);
		FGameplayEffectSpecHandle GameplayEffectSpecHandle =  MakeOutgoingGameplayEffectSpec(*GameplayEffectToApply, OwnerCharacter->GetCharacterLevel());

		FString ContextString;
		FRealCurve* DamageCurve		= DamageLevelCurve->FindCurve(DamageCurveName, ContextString);
		FRealCurve* ScalingCurve	= DamageLevelCurve->FindCurve(AttributeScalingCurveName, ContextString);

		FGameplayEffectContextHandle EffectContext			= MakeEffectContext(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo());
		FRGX_GameplayEffectContext* GameplayEffectContext	= static_cast<FRGX_GameplayEffectContext*>(EffectContext.Get());
		GameplayEffectContext->DamageAmount					= DamageCurve->Eval(TargetCharacter->GetCharacterLevel());
		GameplayEffectContext->ScalingAttributeFactor		= ScalingCurve->Eval(TargetCharacter->GetCharacterLevel());

		FGameplayEffectSpec* GESpec = GameplayEffectSpecHandle.Data.Get();
		GESpec->SetContext(EffectContext);
		TargetACS->ApplyGameplayEffectSpecToSelf(*GESpec);

		// Apply the effect given the TargetData
		//FGameplayAbilitySpecHandle AbilitySpecHandle	= GetCurrentAbilitySpecHandle();
		//FGameplayAbilityActorInfo ActorInfo				= GetActorInfo();
		//FGameplayAbilityActivationInfo ActivationInfo	= GetCurrentActivationInfo();
		//ApplyGameplayEffectSpecToTarget(AbilitySpecHandle, &ActorInfo, ActivationInfo, GameplayEffectSpecHandle, EventData.TargetData);
	}
}
