#include "RGX_MageAngelDieAbility.h"

#include "Kismet/GameplayStatics.h"

#include "../../Actors/Enemies/RGX_MageAngel.h"

void URGX_MageAngelDieAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	const AActor* AvatarActor = GetAvatarActorFromActorInfo();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystem, AvatarActor->GetActorLocation() );

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void URGX_MageAngelDieAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (ARGX_MageAngel* MageAngel = Cast<ARGX_MageAngel>(ActorInfo->AvatarActor))
	{
		MageAngel->SetToBeDestroyed();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
