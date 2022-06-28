#include "RGX_SlowWallAbility.h"

#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RegicideX/Actors/Weapons/RGX_SlowWall.h"

void URGX_SlowWallAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	const AActor* AvatarActor = GetAvatarActorFromActorInfo();
	const ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(AvatarActor);
	const AActor* Player = Enemy->TargetActor;

	const FVector Forward = Player->GetActorForwardVector();
	const FVector WallLocation = Player->GetActorLocation() + Forward*DistanceToTarget;
	const FTransform WallTransform(WallLocation);
	GetWorld()->SpawnActor<ARGX_SlowWall>(WallActorClass, WallTransform);

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}
