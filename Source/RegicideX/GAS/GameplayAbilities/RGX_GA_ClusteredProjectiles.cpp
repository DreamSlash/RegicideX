// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_GA_ClusteredProjectiles.h"
#include "RegicideX\Actors\Enemies\RGX_DistancePeasant.h"
#include "RegicideX\Actors\Weapons\RGX_ClusteredBullet.h"

void URGX_GA_ClusteredProjectiles::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	ARGX_DistancePeasant* DistancePeasant = Cast<ARGX_DistancePeasant>(ActorInfo->AvatarActor);
	USceneComponent* SpawnPoint = DistancePeasant->ClusterSpawnPoint;
	
	const FVector Center		= SpawnPoint->GetComponentLocation();
	const FVector Right			= SpawnPoint->GetRightVector();
	const FVector Forward		= SpawnPoint->GetForwardVector();
	const FVector Up			= SpawnPoint->GetUpVector();
	Transform					= SpawnPoint->GetComponentTransform();
	
	for (int i = 0; i < NumberProjectilesToFire; ++i)
	{
		FVector2D Point2D = FMath::RandPointInCircle(75.0f);
		FVector Position = Point2D.X * Right + Point2D.Y * Up;
		Position = Center + Position;
		PointsToSpawn.Add(Position);
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void URGX_GA_ClusteredProjectiles::OnReceivedEvent(
	FGameplayTag EventTag, 
	FGameplayEventData EventData)
{
	for (const auto& Point : PointsToSpawn)
	{
		Transform.SetTranslation(Point);
		GetWorld()->SpawnActor<ARGX_ClusteredBullet>(BulletBP, Transform);
	}
	PointsToSpawn.Empty();
}
