#include "RGX_GA_DodgeAbility.h"
#include "GameFramework/Character.h"
#include "RegicideX/Character/RGX_PlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayEffect.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

void URGX_DodgeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ARGX_PlayerCharacter* Character = Cast<ARGX_PlayerCharacter>(ActorInfo->AvatarActor);

	if (Character)
	{
		FGameplayEventData EventData;
		ActorInfo->AbilitySystemComponent->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Character.Interrupted")), &EventData);

		Character->GetCharacterMovement()->GravityScale = 0.0f;
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

void URGX_DodgeAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	UAbilitySystemComponent* ACS = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(CurrentActorInfo->AvatarActor.Get());
	if (ACS)
	{
		ACS->ApplyGameplayEffectToSelf(InvulnerabilityEffect.GetDefaultObject(), 1.0f, ACS->MakeEffectContext());
	}

	ARGX_PlayerCharacter* Character = Cast<ARGX_PlayerCharacter>(ActorInfo->AvatarActor);
	if (Character)
	{
		Character->GetCharacterMovement()->MaxAcceleration = Character->MaxAcceleration;
		Character->GetCharacterMovement()->GravityScale = Character->DefaultGravity;
		Character->EnableMovementInput();

		UCapsuleComponent* CapsuleComponent = Character->GetCapsuleComponent();
		if (CapsuleComponent)
		{
			CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
		}
	}
}