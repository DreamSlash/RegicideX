// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/GAS/AbilityTasks/RGX_MoveToLocationCharge.h"

URGX_MoveToLocationCharge* URGX_MoveToLocationCharge::MoveToLocationCharge(UGameplayAbility* OwningAbility, FName TaskInstanceName, FVector Location, float Duration, UCurveFloat* OptionalInterpolationCurve, UCurveVector* OptionalVectorInterpolationCurve)
{
	URGX_MoveToLocationCharge* MyObj = NewAbilityTask<URGX_MoveToLocationCharge>(OwningAbility, TaskInstanceName);

	if (MyObj->GetAvatarActor() != nullptr)
	{
		MyObj->StartLocation = MyObj->GetAvatarActor()->GetActorLocation();
	}

	MyObj->TargetLocation = Location;
	MyObj->DurationOfMovement = FMath::Max(Duration, 0.001f);		// Avoid negative or divide-by-zero cases
	MyObj->TimeMoveStarted = MyObj->GetWorld()->GetTimeSeconds();
	MyObj->TimeMoveWillEnd = MyObj->TimeMoveStarted + MyObj->DurationOfMovement;
	MyObj->LerpCurve = OptionalInterpolationCurve;
	MyObj->LerpCurveVector = OptionalVectorInterpolationCurve;

	return MyObj;
}

void URGX_MoveToLocationCharge::Activate()
{
	Super::Activate();
}

void URGX_MoveToLocationCharge::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("Manuela Task\n"));
}

void URGX_MoveToLocationCharge::OnDestroy(bool AbilityIsEnding)
{
	Super::OnDestroy(AbilityIsEnding);
}
