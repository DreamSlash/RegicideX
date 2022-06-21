// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/GAS/AbilityTasks/RGX_AT_MoveToLocationCharge.h"
#include "Curves/CurveFloat.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Curves/CurveVector.h"
#include "RegicideX/Actors/Enemies/RGX_MeleeAngel.h"

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
	// This function is the same as the parent class except it sets a charge velocity in the actor to be able to get its velocity
	// correctly for the player to dodge.

	if (bIsFinished)
	{
		return;
	}

	AActor* MyActor = GetAvatarActor();
	if (MyActor)
	{
		const FVector LastLocation = MyActor->GetActorLocation();

		ACharacter* MyCharacter = Cast<ACharacter>(MyActor);
		if (MyCharacter)
		{
			UCharacterMovementComponent* CharMoveComp = Cast<UCharacterMovementComponent>(MyCharacter->GetMovementComponent());
			if (CharMoveComp)
			{
				CharMoveComp->SetMovementMode(MOVE_Custom, 0);
			}
		}


		float CurrentTime = GetWorld()->GetTimeSeconds();

		if (CurrentTime >= TimeMoveWillEnd)
		{
			bIsFinished = true;

			// Teleport in attempt to find a valid collision spot
			MyActor->TeleportTo(TargetLocation, MyActor->GetActorRotation());
			if (!bIsSimulating)
			{
				MyActor->ForceNetUpdate();
				if (ShouldBroadcastAbilityTaskDelegates())
				{
					OnTargetLocationReached.Broadcast();
				}
				EndTask();
			}
		}
		else
		{
			FVector NewLocation;

			float MoveFraction = (CurrentTime - TimeMoveStarted) / DurationOfMovement;
			if (LerpCurveVector)
			{
				const FVector ComponentInterpolationFraction = LerpCurveVector->GetVectorValue(MoveFraction);
				NewLocation = FMath::Lerp<FVector, FVector>(StartLocation, TargetLocation, ComponentInterpolationFraction);
			}
			else
			{
				if (LerpCurve)
				{
					MoveFraction = LerpCurve->GetFloatValue(MoveFraction);
				}

				NewLocation = FMath::Lerp<FVector, float>(StartLocation, TargetLocation, MoveFraction);
			}

			MyActor->SetActorLocation(NewLocation);

			// TODO: hardcoded to melee angel but this class should be a base character class shared by all characters in the game
			ARGX_MeleeAngel* MEAngel = Cast<ARGX_MeleeAngel>(MyActor);
			if (MEAngel)
			{
				const FVector LocationDelta = NewLocation - LastLocation;
				const FVector Velocity = LocationDelta / DeltaTime;
				UE_LOG(LogTemp, Warning, TEXT("Charge Velocity: %f, %f, %f\n"), Velocity.X, Velocity.Y, Velocity.Z);
				MEAngel->SetChargeVelocity(Velocity);
			}
		}
	}
	else
	{
		bIsFinished = true;
		EndTask();
	}

	UE_LOG(LogTemp, Warning, TEXT("Manuela Task\n"));
}

void URGX_MoveToLocationCharge::OnDestroy(bool AbilityIsEnding)
{
	Super::OnDestroy(AbilityIsEnding);
}
