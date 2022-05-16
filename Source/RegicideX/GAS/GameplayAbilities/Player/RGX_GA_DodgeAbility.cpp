#include "RGX_GA_DodgeAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RegicideX/Character/RGX_PlayerCharacter.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

void URGX_DodgeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Warning, TEXT("Activate Dodge\n"));

	ARGX_PlayerCharacter* Character = Cast<ARGX_PlayerCharacter>(ActorInfo->AvatarActor);
	URGX_CombatAssistComponent* CombatAssistComponent = Character->FindComponentByClass<URGX_CombatAssistComponent>();

	if (Character)
	{
		FGameplayEventData EventData;
		ActorInfo->AbilitySystemComponent->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Character.Interrupted")), &EventData);

		Character->GetCharacterMovement()->GravityScale = 0.0f;
		FVector MoveDirection = Character->GetVelocity();

		if (MoveDirection.Size() <= 10.0f)
		{
			MoveDirection = -Character->GetActorForwardVector();
		}

		MoveDirection.Normalize();

		Character->DisableMovementInput();
		CombatAssistComponent->EnableMovementVector();
		CombatAssistComponent->AddMovementVector(MoveDirection, DodgeSpeed);

		FName SectionToPlay;

		float Velocity = Character->GetVelocity().Size();
		UE_LOG(LogTemp, Warning, TEXT("Velocity: %f"), Velocity);
		if (Velocity <= 10.0f)
		{
			SectionToPlay = BackDodgeSection;
		}
		else
		{
			SectionToPlay = SideDodgeSection;
		}

		UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, MontageToPlay, 1.0f, SectionToPlay);
		PlayMontageTask->OnBlendOut.AddDynamic(this, &URGX_DodgeAbility::FinishDodge);
		PlayMontageTask->OnCancelled.AddDynamic(this, &URGX_DodgeAbility::FinishDodge);
		PlayMontageTask->OnCompleted.AddDynamic(this, &URGX_DodgeAbility::FinishDodge);
		PlayMontageTask->OnInterrupted.AddDynamic(this, &URGX_DodgeAbility::FinishDodge);
		PlayMontageTask->ReadyForActivation();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
	}
}

void URGX_DodgeAbility::FinishDodge()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void URGX_DodgeAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UE_LOG(LogTemp, Warning, TEXT("End Dodge\n"));

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ARGX_PlayerCharacter* Character = Cast<ARGX_PlayerCharacter>(ActorInfo->AvatarActor);
	URGX_CombatAssistComponent* CombatAssistComponent = Character->FindComponentByClass<URGX_CombatAssistComponent>();

	Character->GetCharacterMovement()->GravityScale = Character->DefaultGravity;
	CombatAssistComponent->DisableMovementVector();
	CombatAssistComponent->RemoveMovementVector();
	Character->EnableMovementInput();
}