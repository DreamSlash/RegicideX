#include "RGX_SlowWallAbility.h"

#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RegicideX/Actors/Weapons/RGX_SlowWall.h"

void URGX_SlowWallAbility::OnAttackWindow()
{
	const AActor* AvatarActor = GetAvatarActorFromActorInfo();
	const ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(AvatarActor);
	const AActor* Player = Enemy->TargetActor;

	const FVector Forward = Player->GetActorForwardVector();
	const FVector WallLocation = Player->GetActorLocation() + Forward * DistanceToTarget;
	const FTransform WallTransform(WallLocation);
	GetWorld()->SpawnActor<ARGX_SlowWall>(WallActorClass, WallTransform);
}
