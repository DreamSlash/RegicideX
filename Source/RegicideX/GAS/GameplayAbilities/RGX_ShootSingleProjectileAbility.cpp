// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_ShootSingleProjectileAbility.h"

#include "RegicideX\Actors\Weapons\RGX_Bullet.h"


URGX_ShootSingleProjectileAbility::URGX_ShootSingleProjectileAbility() 
{

}

void URGX_ShootSingleProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Shoot(Cast<AActor>(ActorInfo->AvatarActor));

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void URGX_ShootSingleProjectileAbility::Shoot(AActor* Actor)
{
	const FVector ProjectileLocation = Actor->GetActorLocation() + Actor->GetActorForwardVector() * 200.0;

	FTransform BulletTransform(FRotator(),ProjectileLocation, FVector(0.1));

	GetWorld()->SpawnActor<ARGX_Bullet>(ProjectileClass, BulletTransform);
}
