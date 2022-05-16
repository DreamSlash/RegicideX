#include "RGX_GA_JumpAbility.h"
#include "GameFramework/Character.h"
#include "../../../Character/RGX_PlayerCharacter.h"

void URGX_JumpAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor);

	ARGX_PlayerCharacter* PlayerCharacter = Cast<ARGX_PlayerCharacter>(Character);

	if (PlayerCharacter)
	{
		PlayerCharacter->OnJump();
	}

	if (Character)
	{
		Character->Jump();
	}

	UE_LOG(LogTemp, Warning, TEXT("Jump Ability"));
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}
