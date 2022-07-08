#include "RGX_PlayerFallAttackAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RegicideX/Character/RGX_PlayerCharacter.h"
#include "RegicideX/Character/RGX_PlayerCharacterController.h"
#include "RegicideX/Components/RGX_CombatAssistComponent.h"
#include "RegicideX/Components/RGX_HitboxesManagerComponent.h"
#include "RegicideX/Components/RGX_HitboxComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

URGX_PlayerFallAttackAbility::URGX_PlayerFallAttackAbility()
{
}

bool URGX_PlayerFallAttackAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool Result = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (Result == false)
		return false;

	ARGX_PlayerCharacter* PlayerCharacter = Cast<ARGX_PlayerCharacter>(ActorInfo->AvatarActor);
	if (PlayerCharacter == nullptr)
		return false;

	URGX_HitboxesManagerComponent* HitboxesManagerComponent = PlayerCharacter->FindComponentByClass<URGX_HitboxesManagerComponent>();
	if (HitboxesManagerComponent == nullptr)
		return false;

	return true;
}

void URGX_PlayerFallAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ARGX_PlayerCharacter* PlayerCharacter = Cast<ARGX_PlayerCharacter>(ActorInfo->AvatarActor);
	PlayerCharacter->DisableMovementInput();

	ARGX_PlayerCharacterController* PlayerController = Cast<ARGX_PlayerCharacterController>(ActorInfo->PlayerController);
	PlayerController->OnStartFallAttack();

	UE_LOG(LogTemp, Warning, TEXT("Activate Fall Attack\n"));
}

void URGX_PlayerFallAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ARGX_PlayerCharacter* PlayerCharacter = Cast<ARGX_PlayerCharacter>(CurrentActorInfo->AvatarActor);
	PlayerCharacter->EnableMovementInput();

	ARGX_PlayerCharacterController* PlayerController = Cast<ARGX_PlayerCharacterController>(ActorInfo->PlayerController);
	PlayerController->OnEndFallAttack();

	UE_LOG(LogTemp, Warning, TEXT("End fall attack\n"));

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URGX_PlayerFallAttackAbility::OnReceivedEvent(FGameplayTag EventTag, FGameplayEventData EventData)
{
	AActor* OwnerActor = GetOwningActorFromActorInfo();
	ARGX_CharacterBase* OwnerCharacter = Cast<ARGX_CharacterBase>(OwnerActor);
	UAbilitySystemComponent* OwnerACS = OwnerCharacter->GetAbilitySystemComponent();
	const ARGX_CharacterBase* TargetCharacter = Cast<ARGX_CharacterBase>(EventData.Target);
	UAbilitySystemComponent* TargetACS = TargetCharacter ? TargetCharacter->GetAbilitySystemComponent() : nullptr;

	// Find the effect mapped to the triggering event tag to apply to target.
	if (EffectToApplyToTarget.Contains(EventTag) && TargetACS)
	{
		TSubclassOf<UGameplayEffect>* GameplayEffectToApply = EffectToApplyToTarget.Find(EventTag);
		FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeOutgoingGameplayEffectSpec(*GameplayEffectToApply, OwnerCharacter->GetCharacterLevel());

		// TODO Damage should be handled by the effect and not the ability.
		FString ContextString;
		FRealCurve* DamageCurve = DamageLevelCurve->FindCurve(DamageCurveName, ContextString);
		FRealCurve* ScalingCurve = DamageLevelCurve->FindCurve(AttributeScalingCurveName, ContextString);

		FGameplayEffectContextHandle EffectContext = MakeEffectContext(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo());
		FRGX_GameplayEffectContext* GameplayEffectContext = static_cast<FRGX_GameplayEffectContext*>(EffectContext.Get());
		GameplayEffectContext->DamageAmount = DamageCurve->Eval(TargetCharacter->GetCharacterLevel());
		GameplayEffectContext->ScalingAttributeFactor = ScalingCurve->Eval(TargetCharacter->GetCharacterLevel());

		FGameplayEffectSpec* GESpec = GameplayEffectSpecHandle.Data.Get();
		GESpec->SetContext(EffectContext);
		TargetACS->ApplyGameplayEffectSpecToSelf(*GESpec);
	}

	// Apply own effects, such as cooldowns.
	if (EffectToApplyToOwner.Contains(EventTag) && OwnerACS)
	{
		TSubclassOf<UGameplayEffect>* GameplayEffectToApply = EffectToApplyToOwner.Find(EventTag);
		FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeOutgoingGameplayEffectSpec(*GameplayEffectToApply, OwnerCharacter->GetCharacterLevel());

		OwnerACS->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());
	}
}