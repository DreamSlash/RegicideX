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

float ARGX_PlayerCameraManager::GetAngleFrom2DDirection(FVector2D Direction)
{
	float Angle = FMath::RadiansToDegrees(FMath::Acos(Direction.Y));
	Angle = Direction.X < 0.0f ? 360.0f - Angle : Angle;
	UE_LOG(LogTemp, Warning, TEXT("Angle: %f\n"), Angle);
	return Angle;
}


void ARGX_PlayerCameraManager::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);
}

void ARGX_PlayerCameraManager::DoUpdateCamera(float DeltaTime)
{
	Super::DoUpdateCamera(DeltaTime);
}

void ARGX_PlayerCameraManager::NotifyInput()
{
	LastManualRotationTime = GetWorld()->TimeSeconds;
	UE_LOG(LogTemp, Warning, TEXT("Camera Input\n"));
}

void ARGX_PlayerCameraManager::ProcessViewRotation(float DeltaTime, FRotator& OutViewRotation, FRotator& OutDeltaRot)
{
	const float DeltaRotMagnitude = FMath::Abs(OutDeltaRot.Euler().Size());
	if (DeltaRotMagnitude > 0.0001f == true)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Camera Rotator: %f, %f, %f\n"), OutDeltaRot.Roll, OutDeltaRot.Pitch, OutDeltaRot.Yaw);
		NotifyInput();
	}

	// TODO: Should not autorotate when colliding with obstacles
	// Automatic alignment if no input is received from the player in align delay time
	if (GetWorld()->TimeSeconds - LastManualRotationTime < AlignDelay == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Automatic Alignment\n"));
		FVector2D Movement = FVector2D(
			FocusLocation.Y - PreviousFocusLocation.Y,
			FocusLocation.X - PreviousFocusLocation.X);

		float MovementDeltaSqr = Movement.SizeSquared();
		if (MovementDeltaSqr < 0.000001f == false)
		{
			float HeadingAngle = GetAngleFrom2DDirection(Movement / FMath::Sqrt(MovementDeltaSqr));
			float RotationChange = AutomaticRotationSpeed * DeltaTime;

			OutViewRotation.Yaw = FMath::Lerp(OutViewRotation.Yaw, HeadingAngle, RotationChange);
		}
	}

	Super::ProcessViewRotation(DeltaTime, OutViewRotation, OutDeltaRot);
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
	
	// TODO: Refactor i que no sembli copiat

	PreviousFocusLocation = FocusLocation;

	FVector TargetOffset = SpringArmComponent->TargetOffset;
	FTViewTarget TargetView = OutVT;

	const FVector TargetLocation = TargetView.Target->GetActorLocation();
	
	if (FocusRadius > 0.0f)
	{
		float Distance = FVector::Distance(TargetLocation, FocusLocation);
		//UE_LOG(LogTemp, Warning, TEXT("Camera Distance to Target: %f\n"), Distance);

		float t = 1.0f;
		if (Distance > 0.01f && FocusCentering > 0.0f)
		{
			t = FMath::Pow(1 - FocusCentering, DeltaTime);
		}

		if (Distance > FocusRadius)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Manuela\n"));
			t = FMath::Min(t, FocusRadius / Distance);
		}

		FocusLocation = FMath::Lerp(TargetLocation, FocusLocation, t);
		TargetOffset = FocusLocation - TargetLocation;
	}
	else
	{
		FocusLocation = TargetLocation;
	}
	
	SpringArmComponent->TargetOffset = TargetOffset;
}
