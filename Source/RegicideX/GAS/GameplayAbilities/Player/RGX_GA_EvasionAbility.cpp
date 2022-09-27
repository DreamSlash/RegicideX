#include "RGX_GA_EvasionAbility.h"
#include "GameFramework/Character.h"
#include "RegicideX/Character/RGX_PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

void URGX_EvasionAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ARGX_PlayerCharacter* PlayerCharacter = Cast<ARGX_PlayerCharacter>(ActorInfo->AvatarActor);
	if (PlayerCharacter == false)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
	}

	PlayerCharacter->RotatePlayerTowardsInput();

	bool bIsBeingAttacked = PlayerCharacter->IsBeingAttacked();

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