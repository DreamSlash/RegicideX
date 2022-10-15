// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_AT_RotateAndMoveCharacter.h"
#include "AbilitySystemComponent.h"
#include "RegicideX/GAS/RGX_GameplayEffectContext.h"
#include "RegicideX/Components/RGX_HitboxComponent.h"
#include "RegicideX/Actors/Enemies/RGX_MeleeAngel.h"
#include "RegicideX/RGX_BlueprintLibrary.h"
#include "AbilitySystemGlobals.h"



URGX_AT_RotateAndMoveCharacter::URGX_AT_RotateAndMoveCharacter()
{
	bTickingTask = true;
}

FString URGX_AT_RotateAndMoveCharacter::GetDebugString() const
{
	return FString("Hey!");
}

void URGX_AT_RotateAndMoveCharacter::TickTask(float DeltaTime)
{
	TaskTime += DeltaTime;
	if (TaskTime >= MaxTime)
	{
		Attacker->BHHitboxComponent->DeactivateHitbox();
		Attacker->bTornadoActive = false;
		Attacker->SetTornadoFXVisibility(false);
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnFinish.Broadcast();
		}
		EndTask();
	}
	Attacker->RotateToTarget(DeltaTime);

	FVector TargetLocation = Attacker->TargetActor->GetActorLocation();
	if (FVector::Dist(Attacker->GetActorLocation(), TargetLocation) < 200.0)
		return;

	const FVector NewLocation = Attacker->GetActorLocation() + Attacker->GetActorForwardVector() * Attacker->MoveSpeed * DeltaTime;
	Attacker->SetActorLocation(NewLocation);
}

void URGX_AT_RotateAndMoveCharacter::OnDestroy(bool AbilityIsEnding)
{
	Super::OnDestroy(AbilityIsEnding);
}

URGX_AT_RotateAndMoveCharacter* URGX_AT_RotateAndMoveCharacter::RotateAndMoveCharacter(UGameplayAbility* OwningAbility, float Time)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Duration(Time);
	URGX_AT_RotateAndMoveCharacter* MyObj = NewAbilityTask<URGX_AT_RotateAndMoveCharacter>(OwningAbility);
	MyObj->MaxTime = Time;
	MyObj->Attacker = Cast<ARGX_MeleeAngel>(OwningAbility->GetAvatarActorFromActorInfo());
	MyObj->Attacker->bTornadoActive = true;
	MyObj->Attacker->BHHitboxComponent->ActivateHitbox(true);
	MyObj->Attacker->SetTornadoFXVisibility(true);
	return MyObj;
}
