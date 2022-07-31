// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_AT_RotateAndUpdateForceField.h"

#include "AbilitySystemGlobals.h"
#include "RegicideX\Actors\Enemies\RGX_DistanceAngel.h"
#include "Components/SphereComponent.h"
#include "AbilitySystemComponent.h"
#include "RegicideX/GAS/RGX_GameplayEffectContext.h"
#include "RegicideX/Components/RGX_HitboxComponent.h"

URGX_AT_RotateAndUpdateForceField::URGX_AT_RotateAndUpdateForceField()
{

	bTickingTask = true;

}

FString URGX_AT_RotateAndUpdateForceField::GetDebugString() const
{
	return FString("Hey!");
}

void URGX_AT_RotateAndUpdateForceField::TickTask(float DeltaTime)
{

	TaskTime += DeltaTime;
	if (TaskTime >= MaxTime)
	{
		TaskTime = 0.0f;
		Attacker->BulletHellSphere->SetRelativeScale3D(FVector(OriginalBulletHellSphereScale));
		Attacker->BulletHellSphere->SetHiddenInGame(true);
		Attacker->BulletHellSphereCollider->SetRelativeScale3D(FVector(6.5));
		Attacker->BHHitboxComponent->DeactivateHitbox();
		//Attacker->BHHitboxComponent->DeactivateEffect();
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnFinish.Broadcast();
		}
		EndTask();
	}

	Attacker->RotateMe(DeltaTime, 1.0 + TaskTime * RotationSpeedMultiplier);

	FVector CurrentScale = Attacker->BulletHellSphere->GetRelativeScale3D();
	if (CurrentScale.Z < MaxSize)
	{
		FVector NewScale = Attacker->BulletHellSphere->GetRelativeScale3D() + FVector(TaskTime) * 0.5;
		Attacker->BulletHellSphere->SetRelativeScale3D(NewScale);
		FVector NewScale2 = Attacker->BulletHellSphereCollider->GetRelativeScale3D() + FVector(TaskTime) * 0.8;
		Attacker->BulletHellSphereCollider->SetRelativeScale3D(NewScale2);
	}

}



void URGX_AT_RotateAndUpdateForceField::OnDestroy(bool AbilityIsEnding)
{
	Super::OnDestroy(AbilityIsEnding);
}

URGX_AT_RotateAndUpdateForceField* URGX_AT_RotateAndUpdateForceField::RotateAndUpdateForceField(UGameplayAbility* OwningAbility, float Time, float MaxSize)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Duration(Time);
	URGX_AT_RotateAndUpdateForceField* MyObj = NewAbilityTask<URGX_AT_RotateAndUpdateForceField>(OwningAbility);
	MyObj->MaxTime = Time;
	MyObj->Attacker = Cast<ARGX_DistanceAngel>(OwningAbility->GetAvatarActorFromActorInfo());
	MyObj->MaxSize = MaxSize;
	MyObj->Attacker->BulletHellSphereCollider->SetRelativeScale3D(FVector(10.0));
	MyObj->Attacker->BulletHellSphere->SetHiddenInGame(false);
	MyObj->Attacker->BulletHellOutSphere->SetRelativeScale3D(FVector(MaxSize));
	MyObj->Attacker->BHHitboxComponent->ActivateHitbox();
	//Attacker->BHHitboxComponent->ActivateEffect();
	return MyObj;
}
