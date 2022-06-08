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

	URGX_HitboxesManagerComponent* HitboxManagerComponent = PlayerCharacter->FindComponentByClass<URGX_HitboxesManagerComponent>();
	URGX_HitboxComponent* Hitbox = HitboxManagerComponent->GetHitboxByTag(HitboxTag);
	Hitbox->RemoveAbilityEffectsInfo();
	UE_LOG(LogTemp, Warning, TEXT("End fall attack\n"));

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URGX_PlayerFallAttackAbility::OnSuccessfulAbilityMontage(FGameplayTag EventTag, FGameplayEventData EventData)
{
	Super::OnSuccessfulAbilityMontage(EventTag, EventData);
}

void URGX_PlayerFallAttackAbility::OnFailedAbilityMontage(FGameplayTag EventTag, FGameplayEventData EventData)
{
	Super::OnFailedAbilityMontage(EventTag, EventData);
}

void URGX_PlayerFallAttackAbility::OnReceivedEvent(FGameplayTag EventTag, FGameplayEventData EventData)
{
	Super::OnReceivedEvent(EventTag, EventData);
}

void URGX_PlayerFallAttackAbility::OnFinalMontageFinished()
{
	Super::OnFinalMontageFinished();
}

void URGX_PlayerFallAttackAbility::PopulateGameplayEffectContext(FRGX_GameplayEffectContext& GameplayEffectContext)
{
	float AbilityLevel;
	ARGX_PlayerCharacter* PlayerCharacter = Cast<ARGX_PlayerCharacter>(CurrentActorInfo->AvatarActor);
	if (PlayerCharacter)
	{
		AbilityLevel = static_cast<float>(PlayerCharacter->Level);
	}
	else
	{
		AbilityLevel = GetAbilityLevel();
	}

	FString ContextString;
	FRealCurve* DamageCurve = DamageLevelCurve->FindCurve(DamageCurveName, ContextString);
	FRealCurve* ScalingCurve = DamageLevelCurve->FindCurve(AttributeScalingCurveName, ContextString);
	GameplayEffectContext.DamageAmount = DamageCurve->Eval(AbilityLevel);
	GameplayEffectContext.ScalingAttributeFactor = ScalingCurve->Eval(AbilityLevel);
}

