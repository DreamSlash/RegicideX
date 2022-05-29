#include "RGX_GA_DashAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../../Character/RGX_PlayerCharacter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

void URGX_DashAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ARGX_PlayerCharacter* Character = Cast<ARGX_PlayerCharacter>(ActorInfo->AvatarActor);

	if (Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("Dash Ability"));

		if (Character->GetCharacterMovement()->IsFalling())
		{
			if (bool bHasAirDashed = Character->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.HasAirDashed"))))
			{
				EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
				return;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Air Dash\n"));
				Character->StopJumping();
				Character->LaunchCharacter(FVector(0.0f, 0.0f, 0.0f), false, true);
				Character->AddGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.HasAirDashed")));
			}
		}

		Character->GetCharacterMovement()->GravityScale = 0.0f;
		Character->GetCharacterMovement()->MaxWalkSpeed = DashSpeed;

		UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, MontageToPlay);
		PlayMontageTask->OnBlendOut.AddDynamic(this, &URGX_DashAbility::FinishDash);
		PlayMontageTask->OnCancelled.AddDynamic(this, &URGX_DashAbility::FinishDash);
		PlayMontageTask->OnCompleted.AddDynamic(this, &URGX_DashAbility::FinishDash);
		PlayMontageTask->OnInterrupted.AddDynamic(this, &URGX_DashAbility::FinishDash);
		PlayMontageTask->ReadyForActivation();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
	}
}

void URGX_DashAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ARGX_PlayerCharacter* Character = Cast<ARGX_PlayerCharacter>(ActorInfo->AvatarActor);

	Character->GetCharacterMovement()->GravityScale = Character->DefaultGravity;
	Character->GetCharacterMovement()->MaxWalkSpeed = Character->MaxWalkSpeed;

	UE_LOG(LogTemp, Warning, TEXT("Finish Dash. Current Gravity: %f\n"), Character->GetCharacterMovement()->GravityScale);
}

void URGX_DashAbility::FinishDash()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
