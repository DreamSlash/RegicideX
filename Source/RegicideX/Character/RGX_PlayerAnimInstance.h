// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RGX_PlayerAnimInstance.generated.h"

USTRUCT()
struct FRGX_LeanInfo
{
	GENERATED_BODY()

public:
	UPROPERTY()
	float LeanAmount;

	UPROPERTY(EditAnywhere)
	float InterSpeed;
};

/**
 *
 */
UCLASS()
class REGICIDEX_API URGX_PlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:	
	UPROPERTY(BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bDesiresToJump;

	UPROPERTY(BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
	bool bWasHit;

private:
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	class ARGX_PlayerCharacter* PlayerCharacter;
	
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float MovementSpeed;
	
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Direction;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsOnAir;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float YawChange;

	float LeanAmount;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float LeanValue;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float LeanOffset = 100.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float LeanInterpSpeed = 10.0f;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsBraking;

	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	bool bIsAttacking;

	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	bool bIsDashing;

	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	bool bIsAlive;

	FRotator CharacterRotationLastFrame;
	FRotator CharacterRotation;
	FRotator DeltaRotation;

private:
	float CalculateLeanAmount(float DeltaSeconds);
};
