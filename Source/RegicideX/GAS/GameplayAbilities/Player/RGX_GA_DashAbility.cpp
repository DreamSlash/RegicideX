#include "RGX_GA_DashAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RegicideX/Character/RGX_PlayerCharacter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Components/CapsuleComponent.h"

void URGX_DashAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ARGX_PlayerCharacter* PlayerCharacter = Cast<ARGX_PlayerCharacter>(ActorInfo->AvatarActor);

	if (PlayerCharacter)
	{
		PlayerCharacter->DisableMovementInput();

		UCharacterMovementComponent* CharacterMovementComponent = PlayerCharacter->GetCharacterMovement();
		if (CharacterMovementComponent)
		{
			CharacterMovementComponent->MaxAcceleration = 99999999.0f;
			CharacterMovementComponent->GravityScale = 0.0f;

			if (PlayerCharacter->GetCharacterMovement()->IsFalling())
			{
				if (bool bHasAirDashed = PlayerCharacter->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.HasAirDashed"))))
				{
					EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
					return;
				}
				else
				{
					PlayerCharacter->StopJumping();
					PlayerCharacter->LaunchCharacter(FVector(0.0f, 0.0f, 0.0f), false, true);
					// This tag is never removed so air dash con only be made once
					PlayerCharacter->AddGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.HasAirDashed")));
				}
			}
		}

		// Remove collisions with pawns
		UCapsuleComponent* CapsuleComponent = PlayerCharacter->GetCapsuleComponent();
		if (CapsuleComponent)
		{
			CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		}

		// Apply invulnerability effect
		UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();
		if (ASC != nullptr)
		{
			FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeOutgoingGameplayEffectSpec(InvulnerabilityEffect, 1);
			FGameplayEffectContextHandle EffectContext = MakeEffectContext(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo());
			ASC->ApplyGameplayEffectToSelf(InvulnerabilityEffect->GetDefaultObject<UGameplayEffect>(), 0.0f, ASC->MakeEffectContext());
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

	ARGX_PlayerCharacter* PlayerCharacter = Cast<ARGX_PlayerCharacter>(ActorInfo->AvatarActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->EnableMovementInput();

		UCharacterMovementComponent* CharacterMovementComponent = PlayerCharacter->GetCharacterMovement();
		if (CharacterMovementComponent)
		{
			PlayerCharacter->GetCharacterMovement()->MaxAcceleration = PlayerCharacter->MaxAcceleration;
			PlayerCharacter->GetCharacterMovement()->GravityScale = PlayerCharacter->DefaultGravity;

			if (CharacterMovementComponent->IsFalling())
			{
				PlayerCharacter->LaunchCharacter(FVector(0.0f, 0.0f, -0.1f), true, true);
			}
		}


		UCapsuleComponent* CapsuleComponent = PlayerCharacter->GetCapsuleComponent();
		if (CapsuleComponent)
		{
			CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
		}
	}
}
