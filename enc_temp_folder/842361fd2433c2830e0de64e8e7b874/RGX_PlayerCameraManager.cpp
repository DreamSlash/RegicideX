// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_PlayerCameraManager.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"

ARGX_PlayerCameraManager::ARGX_PlayerCameraManager()
{
}

void ARGX_PlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();

	FocusLocation = ViewTarget.Target->GetActorLocation();

	SpringArmComponent = GetOwningPlayerController()->GetCharacter()->FindComponentByClass<USpringArmComponent>();
	if (SpringArmComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Spring Arm Component in PlayerCameraManager is NULLPTR\n"));
	}
}


void ARGX_PlayerCameraManager::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);
}

void ARGX_PlayerCameraManager::DoUpdateCamera(float DeltaTime)
{
	Super::DoUpdateCamera(DeltaTime);
}

void ARGX_PlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	// Implements different camera styles by default. Is not being used here, our camera view is calculated
	// from UpdateViewTargetInternal()
	Super::UpdateViewTarget(OutVT, DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("%s\n"),*SpringArmComponent->GetName());
}

void ARGX_PlayerCameraManager::UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime)
{
	//Super::UpdateViewTargetInternal(OutVT, DeltaTime);

	if (OutVT.Target)
	{
		FVector OutLocation;
		FRotator OutRotation;
		float OutFOV;

		if (BlueprintUpdateCamera(OutVT.Target, OutLocation, OutRotation, OutFOV))
		{
			OutVT.POV.Location = OutLocation;
			OutVT.POV.Rotation = OutRotation;
			OutVT.POV.FOV = OutFOV;
		}
		else
		{
			OutVT.Target->CalcCamera(DeltaTime, OutVT.POV);
		}
	}
	
	FVector TargetOffset = SpringArmComponent->TargetOffset;
	FTViewTarget TargetView = OutVT;

	const FVector TargetLocation = TargetView.Target->GetActorLocation();
	
	if (FocusRadius > 0.0f)
	{
		float Distance = FVector::Distance(TargetLocation, FocusLocation);
		UE_LOG(LogTemp, Warning, TEXT("Camera Distance to Target: %f\n"), Distance);
		if (Distance > FocusRadius)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Manuela\n"));
			FocusLocation = FMath::Lerp(TargetLocation, FocusLocation, FocusRadius / Distance);
			TargetOffset = FocusLocation - TargetLocation;
		}
		else
		{
			TargetOffset = FocusLocation - TargetLocation;
		}
	}
	else
	{
		FocusLocation = TargetLocation;
	}

	SpringArmComponent->TargetOffset = TargetOffset;
}
