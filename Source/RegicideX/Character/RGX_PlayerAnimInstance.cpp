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

	bIsOnAir = PlayerCharacter->GetCharacterMovement()->IsFalling();
	bIsAttacking = PlayerCharacter->IsAttacking();
	bIsDashing = PlayerCharacter->IsDashing();
	bIsAlive = PlayerCharacter->IsAlive();

	// Character Lean
	YawChange = PlayerCharacter->GetYawChange();
	LeanValue = CalculateLeanAmount(DeltaSeconds) * LeanOffset;
}

float URGX_PlayerAnimInstance::CalculateLeanAmount(float DeltaSeconds)
{
	FRGX_LeanInfo LeanInfo;

	const float YawChangeClamped = UKismetMathLibrary::FClamp(YawChange, -1.0f, 1.0f);
	const bool bInsuficientVelocity = PlayerCharacter->GetCharacterMovement()->IsFalling() || PlayerCharacter->GetVelocity().Size() < 5.0f;

	if (bInsuficientVelocity == true)
	{
		LeanInfo.LeanAmount = 0.0f;
		LeanInfo.InterSpeed = 10.0f;
	}
	else
	{
		LeanInfo.LeanAmount = YawChangeClamped;
		LeanInfo.InterSpeed = 1.0f;
	}

	LeanAmount = UKismetMathLibrary::FInterpTo(LeanAmount, LeanInfo.LeanAmount, DeltaSeconds, LeanInfo.InterSpeed);

	UE_LOG(LogTemp, Warning, TEXT("Lean Amount: %f"), LeanInfo.LeanAmount);

	return LeanAmount;
}