// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_ShootProjectilesAbility.h"
#include "RegicideX\Actors\Enemies\RGX_DistanceAngel.h"
#include "RegicideX\Actors\Weapons\RGX_Bullet.h"

URGX_ShootProjectilesAbility::URGX_ShootProjectilesAbility()
{

}

void URGX_ShootProjectilesAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	Shoot(Cast<ARGX_DistanceAngel>(ActorInfo->AvatarActor));

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void URGX_ShootProjectilesAbility::Shoot(ARGX_DistanceAngel* DistAngel)
{
	int count = DistAngel->SimpleBombsCount;
	float offset = DistAngel->SimpleBombsOffset;


	for (int i = -count; i <= count; i++) 
	{
		FRotator BulletRotation = DistAngel->GetActorRotation();

		float RotYawOffset = FMath::RandRange(-180.0f, 180.0f);
		BulletRotation.Yaw += RotYawOffset;

		float RotPitchOffset = FMath::RandRange(-45.0f, 45.0f);
		BulletRotation.Pitch += RotPitchOffset;

		FVector BulletPosition = DistAngel->GetActorLocation() + FVector(0.0, 0.0, i*offset);
		FVector BulletScale(0.1);
		FTransform BulletTransform(BulletRotation, BulletPosition, BulletScale);

		GetWorld()->SpawnActor<ARGX_Bullet>(ProjectileClass, BulletTransform);
	}
}
