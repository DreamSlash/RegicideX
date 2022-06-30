#include "RGX_SlowWallAbility.h"

#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RegicideX/Actors/Weapons/RGX_SlowWall.h"

void URGX_SlowWallAbility::OnAttackWindow()
{
	const AActor* AvatarActor = GetAvatarActorFromActorInfo();
	const ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(AvatarActor);
	const AActor* Player = Enemy->TargetActor;

	const FVector Forward = Player->GetActorForwardVector();
	UStaticMeshComponent* comp = Cast<UStaticMeshComponent>(Player->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	FVector WallLocation = Player->GetActorLocation() + Forward * DistanceToTarget;
	WallLocation.Z = 100.0f; // Hack duro
	//const FVector WallLocation = comp->GetComponentLocation() + Forward * DistanceToTarget;
	const FRotator Rotator = Player->GetActorRotation();
	const FTransform WallTransform(Rotator.Quaternion(), WallLocation);
	GetWorld()->SpawnActor<ARGX_SlowWall>(WallActorClass, WallTransform);
}
