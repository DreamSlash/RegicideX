
#include "RGX_GA_MeleeAttackAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "RegicideX/Components/RGX_CombatAssistComponent.h"
#include "RegicideX/Character/RGX_PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

URGX_MeleeAttackAbility::URGX_MeleeAttackAbility()
{
	EventTagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Action")));
}

void URGX_MeleeAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ARGX_CharacterBase* CharacterBase = Cast<ARGX_CharacterBase>(GetAvatarActorFromActorInfo());
	if (CharacterBase == nullptr) return;

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
		CombatAssistComponent->RemoveMovementVector();
		CombatAssistComponent->AddMovementVector(ActorInfo->AvatarActor->GetActorForwardVector(), MoveVectorLength, true);

		UCharacterMovementComponent* CharacterMovementComponent = CharacterBase->GetCharacterMovement();
		if (CharacterMovementComponent)
		{
			PreviousMaxAcceleration = CharacterMovementComponent->MaxAcceleration;

			CharacterMovementComponent->MaxAcceleration = 99999999.0f;
		}
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void URGX_MeleeAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ARGX_CharacterBase* CharacterBase = Cast<ARGX_CharacterBase>(GetAvatarActorFromActorInfo());
	if (CharacterBase == nullptr) return;

	ARGX_PlayerCharacter* PlayerCharacter = Cast<ARGX_PlayerCharacter>(CharacterBase);
	if (PlayerCharacter)
	{
		PlayerCharacter->EnableMovementInput();
	}

	UE_LOG(LogTemp, Warning, TEXT("END MELEE ATTACK ABILITY"));

	URGX_CombatAssistComponent* CombatAssistComponent = CurrentActorInfo->AvatarActor->FindComponentByClass<URGX_CombatAssistComponent>();
	if (CombatAssistComponent)
	{
		CombatAssistComponent->RemoveMovementVector();
		CombatAssistComponent->DisableMovementVector();

		UCharacterMovementComponent* CharacterMovementComponent = CharacterBase->GetCharacterMovement();
		if (CharacterMovementComponent)
		{
			CharacterMovementComponent->MaxAcceleration = PreviousMaxAcceleration;
		}
	}
}
