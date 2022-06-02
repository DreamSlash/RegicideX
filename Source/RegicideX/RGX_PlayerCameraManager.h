// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "RGX_PlayerCameraManager.generated.h"

class USpringArmComponent;

/**
 * 
 */
UCLASS()
class REGICIDEX_API ARGX_PlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

private:
	// Not owner of this component.
	USpringArmComponent* SpringArmComponent = nullptr;

public:
	ARGX_PlayerCameraManager();

	void UpdateCamera(float DeltaTime) override;

	void DoUpdateCamera(float DeltaTime) override;

	void NotifyInput(); // function called by the player when generating a camera input

	void SetTargetAngle(const FRotator TargetAngle, const float RotationSpeed, const bool bOverrideRoll, const bool bOverridePitch, const bool bOverrideYaw);

	void ProcessViewRotation(float DeltaTime, FRotator& OutViewRotation, FRotator& OutDeltaRot);

protected:
	void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime);

	void UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime);

protected:
	void BeginPlay() override;

protected:
	float GetAngleFrom2DDirection(FVector2D Direction);

protected:
	FVector FocusLocation;
	FVector PreviousFocusLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerCameraManager)
	float SocketOffsetY = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerCameraManager)
	float TargetOffsetY = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float FocusRadius = 300.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float HorizontalFocusRadius = 300.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float VerticalFocusRadius = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float FocusCentering = 0.5f;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float AlignDelay = 5.0f;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float FallAttackCameraAngle = 45.0f;

	/* Automatic rotation speed in degrees */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float AutomaticRotationSpeed = 2.0f;

	/* Last time the player inputed a camera rotation */
	float LastManualRotationTime;

	/* Attributes to control the rotation to an angle caused by gameplay events */

	/* Flag to mark when the camera is rotation automatically towards some target angle by some gameplay event */
	bool GoingToTargetAngle = false;
	FRotator TargetAngleFromEvent;
	float TargetAngleFromEventSpeed;
	bool bTargetAngleOverrideRoll = false;
	bool bTargetAngleOverridePitch = false;
	bool bTargetAngleOverrideYaw = false;
};