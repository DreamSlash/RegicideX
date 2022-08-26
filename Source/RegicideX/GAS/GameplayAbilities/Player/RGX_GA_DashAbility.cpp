#include "RGX_GA_DashAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RegicideX/Character/RGX_PlayerCharacter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

void URGX_DashAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ARGX_PlayerCharacter* Character = Cast<ARGX_PlayerCharacter>(ActorInfo->AvatarActor);

	if (Character)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Dash Ability"));

		if (Character->GetCharacterMovement()->IsFalling())
		{
			if (bool bHasAirDashed = Character->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.HasAirDashed"))))
			{
				EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
				return;
			}
			else
			{
				//UE_LOG(LogTemp, Warning, TEXT("Air Dash\n"));
				Character->StopJumping();
				Character->LaunchCharacter(FVector(0.0f, 0.0f, 0.0f), false, true);
				// TODO: This tag is never removed so air dash con only be made once
				Character->AddGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.HasAirDashed")));
			}
		}

		Character->GetCharacterMovement()->GravityScale = 0.0f;
		Character->GetCharacterMovement()->MaxWalkSpeed = 0.0f;
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

	Character->GetCharacterMovement()->MaxWalkSpeed = 800.0f;
	Character->GetCharacterMovement()->GravityScale = Character->DefaultGravity;

	//UE_LOG(LogTemp, Warning, TEXT("Finish Dash. Current Gravity: %f\n"), Character->GetCharacterMovement()->GravityScale);
}
