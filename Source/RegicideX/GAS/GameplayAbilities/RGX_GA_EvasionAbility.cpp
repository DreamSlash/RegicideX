#include "RGX_GA_EvasionAbility.h"
#include "GameFramework/Character.h"
#include "../../Character/RGX_PlayerCharacter.h"

void URGX_EvasionAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ARGX_PlayerCharacter* Character = Cast<ARGX_PlayerCharacter>(ActorInfo->AvatarActor);
	if (Character == false)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
	}

	bool bIsBeingAttacked = Character->IsBeingAttacked();

	if (bIsBeingAttacked)
	{
		// Perform dodge ability
		FGameplayEventData EventData;
		ActorInfo->AbilitySystemComponent->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Action.Dodge")), &EventData);
	}
	else
	{
		// Perform dash ability
		FGameplayEventData EventData;
		ActorInfo->AbilitySystemComponent->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Action.Dash")), &EventData);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}
