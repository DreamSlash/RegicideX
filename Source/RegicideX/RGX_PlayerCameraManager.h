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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float FocusRadius = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float FocusCentering = 0.5f;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float AlignDelay = 5.0f;

	float LastManualRotationTime;
};