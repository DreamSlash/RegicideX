#include "RGX_GroundExplosionAbility.h"

#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RegicideX/Actors/Weapons/RGX_GroundExplosion.h"

void URGX_GroundExplosionAbility::OnGround()
{
	Super::OnGround();

	const AActor* AvatarActor = GetAvatarActorFromActorInfo();
	const ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(AvatarActor);
	const AActor* Player = Enemy->TargetActor;

	const FVector ExplosionLocation = Player->GetActorLocation();
	const FTransform ExplosionTransform(ExplosionLocation);
	GetWorld()->SpawnActor<ARGX_GroundExplosion>(ExplosionActorClass, ExplosionTransform);
}