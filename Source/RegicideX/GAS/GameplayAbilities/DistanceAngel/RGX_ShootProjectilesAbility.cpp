// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_ShootProjectilesAbility.h"
#include "RegicideX\Actors\Enemies\RGX_DistanceAngel.h"
#include "RegicideX\Actors\Weapons\RGX_Bullet.h"
#include "RegicideX\Components\RGX_HitboxesManagerComponent.h"

URGX_ShootProjectilesAbility::URGX_ShootProjectilesAbility()
{

}


void URGX_ShootProjectilesAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	CommitAbility(Handle, ActorInfo, ActivationInfo);
	ARGX_DistanceAngel* DistAngel = Cast<ARGX_DistanceAngel>(ActorInfo->AvatarActor);
	Shoot(DistAngel);
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void URGX_ShootProjectilesAbility::Shoot(ARGX_DistanceAngel* DistAngel)
{
	const int count = DistAngel->SimpleBombsCount;
	const float offset = DistAngel->SimpleBombsOffset;

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = DistAngel;

	for (int i = -count; i <= count; ++i) 
	{
		FRotator BulletRotation = DistAngel->GetActorRotation();

		float RotYawOffset = FMath::RandRange(-180.0f, 180.0f);
		BulletRotation.Yaw += RotYawOffset;

		float RotPitchOffset = FMath::RandRange(-45.0f, 45.0f);
		BulletRotation.Pitch += RotPitchOffset;

		const FVector BulletPosition = DistAngel->GetEyeWorldLocation() + FVector(0.0, 0.0, i*offset);
		const FVector BulletScale(0.1);
		FTransform BulletTransform(BulletRotation, BulletPosition, BulletScale);

		//ARGX_Bullet* Bullet = GetWorld()->SpawnActor<ARGX_Bullet>(ProjectileClass, BulletTransform, SpawnParameters);
		//URGX_HitboxesManagerComponent* HitboxComponent = Bullet->FindComponentByClass<URGX_HitboxesManagerComponent>();
		//HitboxComponent->GetHitboxByTag(HitboxTag);
	}
}
