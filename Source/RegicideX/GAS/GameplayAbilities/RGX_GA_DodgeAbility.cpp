#include "RGX_GA_DodgeAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RegicideX/Character/RGX_PlayerCharacter.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"

void URGX_DodgeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Warning, TEXT("Activate Dodge\n"));
	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor);

	PerformDodge();

	UAbilityTask_WaitDelay* WaitDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, DodgeDuration);
	WaitDelayTask->OnFinish.AddDynamic(this, &URGX_DodgeAbility::FinishDodge);
	WaitDelayTask->ReadyForActivation();
}

void URGX_DodgeAbility::PerformDodge()
{
	FGameplayEventData EventData;
	CurrentActorInfo->AbilitySystemComponent->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Character.Interrupted")), &EventData);

	ACharacter* Character = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	FVector Forward = Character->GetActorForwardVector();
	FVector LaunchForce = Forward * DodgeForce;

	Character->GetCharacterMovement()->GravityScale = 0.0f;

	if (Character->GetCharacterMovement()->IsFalling() == true)
	{
		Character->LaunchCharacter(LaunchForce / 2.0f, true, true);
	}
	else
	{
		Character->LaunchCharacter(LaunchForce, true, true);
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

	Character->GetCharacterMovement()->GravityScale = Character->DefaultGravity;
}