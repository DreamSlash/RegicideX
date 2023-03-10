// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_AT_SpawnAndUpdateLaserBeam.h"
#include "AbilitySystemGlobals.h"

#include "RegicideX\Actors\Enemies\RGX_DistanceAngel.h"
#include "RegicideX\Actors\Weapons\RGX_LaserBeamWeapon.h"

URGX_AT_SpawnAndUpdateLaserBeam::URGX_AT_SpawnAndUpdateLaserBeam()
{
	bTickingTask = true;

}

URGX_AT_SpawnAndUpdateLaserBeam* URGX_AT_SpawnAndUpdateLaserBeam::SpawnAndUpdateLaserBeam(UGameplayAbility* OwningAbility, float Time, TSubclassOf<ARGX_LaserBeamWeapon> LaserBeamClass)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Duration(Time);
	URGX_AT_SpawnAndUpdateLaserBeam* MyObj = NewAbilityTask<URGX_AT_SpawnAndUpdateLaserBeam>(OwningAbility);
	MyObj->MaxTime = Time;
	MyObj->Attacker = Cast<ARGX_DistanceAngel>(OwningAbility->GetAvatarActorFromActorInfo());
	MyObj->LaserBeamWeaponSubclass = LaserBeamClass;
	return MyObj;
}

void URGX_AT_SpawnAndUpdateLaserBeam::TickTask(float DeltaTime)
{
	if (LaserBeamWeapon == nullptr) 
	{
		SpawnLaserBeamWeapon();
	}

	if (LaserBeamWeapon->bHittingTarget)
	{
		LaserBeamWeapon->Destroy();
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnFinish.Broadcast();
		}
		EndTask();
	}


	TaskTime += DeltaTime;
	if (TaskTime >= MaxTime)
	{
		TaskTime = 0.0f;
		Attacker->Invincible = false;
		LaserBeamWeapon->Destroy();
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnFinish.Broadcast();
		}
		EndTask();
	}

	if (LaserBeamWeapon->RayType == ERayType::Follow) 
	{
		Attacker->RotateToTarget(DeltaTime);
	}
		
	LaserBeamWeapon->MoveAndDrawRay(DeltaTime);
}

void URGX_AT_SpawnAndUpdateLaserBeam::OnDestroy(bool AbilityIsEnding)
{
	Super::OnDestroy(AbilityIsEnding);
}

void URGX_AT_SpawnAndUpdateLaserBeam::SpawnLaserBeamWeapon()
{
	Attacker->Invincible = true;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Attacker;
	SpawnParams.Instigator = Attacker;
	LaserBeamWeapon = GetWorld()->SpawnActor<ARGX_LaserBeamWeapon>(LaserBeamWeaponSubclass, SpawnParams);
	LaserBeamWeapon->SetOwnerActor(Attacker);
	LaserBeamWeapon->SetSourcePoint(Attacker->GetEyeWorldLocation());
	Attacker->ForceRotateToTarget();
	LaserBeamWeapon->ComputeRayGoal();
}
