#include "RGX_GA_DashAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RegicideX/Character/RGX_PlayerCharacter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Components/CapsuleComponent.h"

void URGX_DashAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ARGX_PlayerCharacter* Character = Cast<ARGX_PlayerCharacter>(ActorInfo->AvatarActor);

	if (Character)
	{
		if (Character->GetCharacterMovement()->IsFalling())
		{
			if (bool bHasAirDashed = Character->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.HasAirDashed"))))
			{
				EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
				return;
			}
			else
			{
				Character->StopJumping();
				Character->LaunchCharacter(FVector(0.0f, 0.0f, 0.0f), false, true);
				// This tag is never removed so air dash con only be made once
				Character->AddGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.HasAirDashed")));
			}
		}

		Character->GetCharacterMovement()->GravityScale = 0.0f;
		Character->GetCharacterMovement()->MaxWalkSpeed = 0.0f;
		Character->DisableMovementInput();

		UCapsuleComponent* CapsuleComponent = Character->GetCapsuleComponent();
		if (CapsuleComponent)
		{
			CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		}
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
	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = 800.0f;
		Character->GetCharacterMovement()->GravityScale = Character->DefaultGravity;
		Character->EnableMovementInput();

		UCapsuleComponent* CapsuleComponent = Character->GetCapsuleComponent();
		if (CapsuleComponent)
		{
			CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
		}
	}
}
