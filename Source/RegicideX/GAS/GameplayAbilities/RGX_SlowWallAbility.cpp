#include "RGX_SlowWallAbility.h"
#include "GameFramework/Character.h"
#include "../../Actors/Enemies/RGX_EnemyBase.h"
#include "../../Actors/Weapons/RGX_SlowWall.h"

void URGX_SlowWallAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	const AActor* AvatarActor = GetAvatarActorFromActorInfo();
	const ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(AvatarActor);
	const AActor* Player = Enemy->TargetActor;

	const FVector Forward = Player->GetActorForwardVector();
	const FVector WallLocation = Player->GetActorLocation() + Forward*DistanceToTarget;
	const FTransform WallTransform(WallLocation);
	GetWorld()->SpawnActor<ARGX_SlowWall>(WallActorClass, WallTransform);

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}
