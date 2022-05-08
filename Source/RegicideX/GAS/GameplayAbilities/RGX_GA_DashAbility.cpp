#include "RGX_GA_DashAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../Character/RGX_PlayerCharacter.h"

/*
void URGX_DashAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ACharacter* character = Cast<ACharacter>(ActorInfo->AvatarActor);
	if (character)
	{
		character->Jump();
	}

	UE_LOG(LogTemp, Warning, TEXT("Jump Ability"));
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}
*/

void URGX_DashAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ARGX_PlayerCharacter* Character = Cast<ARGX_PlayerCharacter>(ActorInfo->AvatarActor);

	Character->GetCharacterMovement()->GravityScale = Character->DefaultGravity;
}

float URGX_DashAbility::GetDashDuration()
{
	return DashDuration;
}

void URGX_DashAbility::PerformDash()
{
	ACharacter* Character = Cast<ACharacter>(CurrentActorInfo->AvatarActor);

	FVector Forward = Character->GetActorForwardVector();

	FVector LaunchForce = Forward * DashForce;

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