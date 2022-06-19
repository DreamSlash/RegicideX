// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_MeleeAngel.h"

#include "Kismet/KismetMathLibrary.h"

#include "RegicideX/Components/RGX_MovementAssistComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

ARGX_MeleeAngel::ARGX_MeleeAngel()
{
	MovementAssistComponent = CreateDefaultSubobject<URGX_MovementAssistComponent>(TEXT("MovementAssistComponent"));

}

void ARGX_MeleeAngel::RotateToTarget(float DeltaTime)
{
	if (TargetActor)
	{
		const FVector MyLocation = this->GetActorLocation();
		const FVector TargetLocation = TargetActor->GetActorLocation();

		FVector ToTarget = TargetLocation - MyLocation;
		ToTarget.Normalize();
		ToTarget.Z = 0.0f;

		FRotator MyTargetRotation = UKismetMathLibrary::MakeRotFromX(ToTarget);
		MyTargetRotation.Pitch = 0.0f;
		MyTargetRotation.Roll = 0.0f;

		const FRotator NewRotation = FMath::Lerp(this->GetActorRotation(), MyTargetRotation, DeltaTime * InterpSpeed);
		this->SetActorRotation(NewRotation);
	}

}

void ARGX_MeleeAngel::SetGravityScale(float value)
{
	GetCharacterMovement()->GravityScale = value;
}

void ARGX_MeleeAngel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}