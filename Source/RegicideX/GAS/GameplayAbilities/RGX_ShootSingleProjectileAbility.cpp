// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_ShootSingleProjectileAbility.h"

#include "RegicideX\Actors\Weapons\RGX_Bullet.h"
#include "RegicideX\Actors\Enemies\RGX_DistanceAngel.h"

URGX_ShootSingleProjectileAbility::URGX_ShootSingleProjectileAbility() 
{

}

void URGX_ShootSingleProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	CommitAbility(Handle, ActorInfo, ActivationInfo);
	Shoot(Cast<APawn>(ActorInfo->AvatarActor));
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void URGX_ShootSingleProjectileAbility::Shoot(APawn* Actor)
{
	ARGX_DistanceAngel* Angel = Cast<ARGX_DistanceAngel>(Actor);
	const FVector ProjectileLocation = Angel->GetEyeWorldLocation() + Angel->GetActorForwardVector() * 200.0;

	FTransform BulletTransform(FRotator(),ProjectileLocation, FVector(0.1));

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = Actor;

	GetWorld()->SpawnActor<ARGX_Bullet>(ProjectileClass, BulletTransform, SpawnParameters);
}
