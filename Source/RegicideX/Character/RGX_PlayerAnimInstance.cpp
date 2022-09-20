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

	const float PlayerLeanAmount = PlayerCharacter->GetLeanAmount();
	LeanValue = PlayerLeanAmount * LeanOffset;

	bIsOnAir = PlayerCharacter->GetCharacterMovement()->IsFalling();
	bIsAttacking = PlayerCharacter->IsAttacking();
	bIsAlive = PlayerCharacter->IsAlive();
}
