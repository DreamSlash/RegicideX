#include "RGX_GA_DashAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../Character/RGX_PlayerCharacter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

void URGX_DashAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ARGX_PlayerCharacter* Character = Cast<ARGX_PlayerCharacter>(ActorInfo->AvatarActor);

	if (Character)
	{
		Character->GetCharacterMovement()->GravityScale = 0.0f;
		Character->GetCharacterMovement()->MaxWalkSpeed = DashSpeed;

		UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, MontageToPlay);
		PlayMontageTask->OnBlendOut.AddDynamic(this, &URGX_DashAbility::FinishDash);
		PlayMontageTask->OnCancelled.AddDynamic(this, &URGX_DashAbility::FinishDash);
		PlayMontageTask->OnCompleted.AddDynamic(this, &URGX_DashAbility::FinishDash);
		PlayMontageTask->OnInterrupted.AddDynamic(this, &URGX_DashAbility::FinishDash);
		PlayMontageTask->ReadyForActivation();
	}

	UE_LOG(LogTemp, Warning, TEXT("Dash Ability"));
}

void URGX_DashAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ARGX_PlayerCharacter* Character = Cast<ARGX_PlayerCharacter>(ActorInfo->AvatarActor);

	Character->GetCharacterMovement()->GravityScale = Character->DefaultGravity;
	Character->GetCharacterMovement()->MaxWalkSpeed = Character->MaxWalkSpeed;
}

void URGX_DashAbility::FinishDash()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
