#include "RGX_GA_DodgeAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../Character/RGX_PlayerCharacter.h"

void URGX_DodgeAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ARGX_PlayerCharacter* Character = Cast<ARGX_PlayerCharacter>(ActorInfo->AvatarActor);

	Character->GetCharacterMovement()->GravityScale = Character->DefaultGravity;
}

float URGX_DodgeAbility::GetDodgeDuration()
{
	return DodgeDuration;
}

void URGX_DodgeAbility::PerformDodge()
{
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