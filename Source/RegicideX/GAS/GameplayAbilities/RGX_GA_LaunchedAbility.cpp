#include "RGX_GA_LaunchedAbility.h"
#include "../RGX_PayloadObjects.h"

void URGX_LaunchedAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	const URGX_LaunchEventPayload* LaunchPayload = Cast<URGX_LaunchEventPayload>(TriggerEventData->OptionalObject);

	FVector LaunchForce = FVector(0.0f, 0.0f, 0.0f);
	if (LaunchPayload)
	{
		LaunchForce = LaunchPayload->LaunchForce;
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}
