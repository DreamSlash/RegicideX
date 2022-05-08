#include "RGX_GroundExplosionAbility.h"

#include "../../Actors/Enemies/RGX_EnemyBase.h"
#include "../../Actors/Weapons/RGX_GroundExplosion.h"

void URGX_GroundExplosionAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	const AActor* AvatarActor = GetAvatarActorFromActorInfo();
	const ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(AvatarActor);
	const AActor* Player = Enemy->TargetActor;

	const FVector ExplosionLocation = Player->GetActorLocation();
	const FTransform ExplosionTransform(ExplosionLocation);
	GetWorld()->SpawnActor<ARGX_GroundExplosion>(ExplosionActorClass, ExplosionTransform);

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}
