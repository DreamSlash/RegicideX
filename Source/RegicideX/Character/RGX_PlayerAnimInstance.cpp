// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_PlayerAnimInstance.h"
#include "RGX_PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void URGX_PlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerCharacter = Cast<ARGX_PlayerCharacter>(TryGetPawnOwner());
}


void URGX_PlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<ARGX_PlayerCharacter>(TryGetPawnOwner());
	}

	if (PlayerCharacter == nullptr) return;

	FVector Velocity = PlayerCharacter->GetVelocity();
	Velocity.Z = 0.0f;
	MovementSpeed = Velocity.Size();

	//const float PlayerLeanAmount = PlayerCharacter->GetLeanAmount();
	//LeanValue = PlayerLeanAmount * LeanOffset;

	bIsOnAir = PlayerCharacter->GetCharacterMovement()->IsFalling();
	bIsAttacking = PlayerCharacter->IsAttacking();
	bIsDashing = PlayerCharacter->IsDashing();
	bIsAlive = PlayerCharacter->IsAlive();

	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = PlayerCharacter->GetActorRotation();
	const FRotator DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = DeltaRotator.Yaw / DeltaSeconds;
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaSeconds, 6.0f);
	const float InterpClamped = FMath::Clamp(Interp, -90.0f, 90.0f);
	LeaningRight = InterpClamped > 0.0f ? true : false;
	//const float InterpClampedAbs = FMath::Abs(InterpClamped);
	Lean = FMath::GetMappedRangeValueClamped(FVector2D(-90.0f, 90.0f), FVector2D(-30.0f, 30.0f), InterpClamped);
	//Lean = InterpClamped;
}
