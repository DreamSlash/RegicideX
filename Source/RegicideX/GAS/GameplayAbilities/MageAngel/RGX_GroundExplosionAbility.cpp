#include "RGX_GroundExplosionAbility.h"

#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RegicideX/Actors/Weapons/RGX_GroundExplosion.h"

void URGX_GroundExplosionAbility::OnGround()
{
	Super::OnGround();

	AActor* avatarActor = GetAvatarActorFromActorInfo();
	APawn* enemy = Cast<APawn>(avatarActor);
	const ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(avatarActor);
	const AActor* Player = Enemy->TargetActor;

	const FVector ExplosionLocation = Player->GetActorLocation();
	const FTransform ExplosionTransform(ExplosionLocation);

	FActorSpawnParameters params;
	params.Instigator = enemy;
	GetWorld()->SpawnActor<ARGX_GroundExplosion>(ExplosionActorClass, ExplosionTransform, params);
}