#include "RGX_GA_DashAbility.h"
#include "GameFramework/Character.h"

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
