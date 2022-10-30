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
	bIsBraking = PlayerCharacter->bIsBraking;
	bIsStrafing = PlayerCharacter->bIsStrafing;

	Direction = CalculateDirection(PlayerCharacter->GetVelocity(), PlayerCharacter->GetActorRotation());
	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = PlayerCharacter->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = Delta.Yaw / DeltaSeconds;
	YawChange = FMath::GetMappedRangeValueClamped(FVector2D(-540.0f, 540.0f), FVector2D(-1.0f, 1.0f), Target);
	LeanValue = FMath::Clamp(CalculateLeanAmount(DeltaSeconds) * LeanOffset, -30.0f, 30.0f);

	CalculateLean();
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
		LeanInfo.InterSpeed = LeanInterpSpeed;
	}

	LeanAmount = UKismetMathLibrary::FInterpTo(LeanAmount, LeanInfo.LeanAmount, DeltaSeconds, LeanInfo.InterSpeed);

	//UE_LOG(LogTemp, Warning, TEXT("Lean Amount: %f"), LeanInfo.LeanAmount);

	return LeanAmount;
}

void URGX_PlayerAnimInstance::CalculateLean()
{
	const FVector velocity = PlayerCharacter->GetVelocity();
	const FRotator rotation = PlayerCharacter->GetActorRotation();

	if (MovementSpeed < 500.0f)
	{
		Lean = 0.0;
	}
	else
	{
		Lean = FMath::Clamp(CalculateDirection(velocity, rotation), -30.0f, 30.0f);
	}

	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Lean: %f"), Lean));*/
}