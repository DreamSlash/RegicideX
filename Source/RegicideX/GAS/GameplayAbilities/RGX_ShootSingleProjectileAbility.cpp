// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_ShootSingleProjectileAbility.h"

#include "RegicideX/Actors/Projectiles/RGX_Projectile.h"
#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RegicideX\Actors\Enemies\RGX_DistanceAngel.h"
#include "GenericTeamAgentInterface.h"

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
	const FVector ProjectileLocation = Angel->GetEyeWorldLocation() + Angel->GetActorForwardVector() * 200.0 + Offset;

	FTransform BulletTransform(FRotator(),ProjectileLocation, FVector(0.1));

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = Actor;

	ARGX_Projectile* Projectile = GetWorld()->SpawnActor<ARGX_Projectile>(ProjectileClass, BulletTransform, SpawnParameters);
	if (Projectile)
	{
		ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(GetAvatarActorFromActorInfo());
		if (Enemy)
		{
			FGenericTeamId TeamID = Enemy->GetGenericTeamId();
			Projectile->SetGenericTeamId(TeamID);
		}

		Projectile->Instigator = GetAvatarActorFromActorInfo();
	}
}
