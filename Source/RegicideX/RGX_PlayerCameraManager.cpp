// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_PlayerCameraManager.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

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
		//UE_LOG(LogTemp, Error, TEXT("Spring Arm Component in PlayerCameraManager is NULLPTR\n"));
	}
}

float ARGX_PlayerCameraManager::GetAngleFrom2DDirection(FVector2D Direction)
{
	float Angle = FMath::RadiansToDegrees(FMath::Acos(Direction.Y));
	Angle = Direction.X < 0.0f ? 360.0f - Angle : Angle;
	//UE_LOG(LogTemp, Warning, TEXT("Angle: %f\n"), Angle);
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
	//UE_LOG(LogTemp, Warning, TEXT("Camera Input\n"));
}

void ARGX_PlayerCameraManager::SetTargetAngle(const FRotator TargetAngle, const float RotationSpeed, const bool bOverrideRoll, const bool bOverridePitch, const bool bOverrideYaw)
{
	GoingToTargetAngle = true;
	TargetAngleFromEvent = TargetAngle;
	TargetAngleFromEventSpeed = RotationSpeed;
	bTargetAngleOverrideRoll = bOverrideRoll;
	bTargetAngleOverridePitch = bOverridePitch;
	bTargetAngleOverrideYaw = bOverrideYaw;
}

void ARGX_PlayerCameraManager::ProcessViewRotation(float DeltaTime, FRotator& OutViewRotation, FRotator& OutDeltaRot)
{
	const float DeltaRotMagnitude = FMath::Abs(OutDeltaRot.Euler().Size());
	if (DeltaRotMagnitude > 0.0001f == true)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Camera Rotator: %f, %f, %f\n"), OutDeltaRot.Roll, OutDeltaRot.Pitch, OutDeltaRot.Yaw);
		NotifyInput();
	}

	if (GoingToTargetAngle == true)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Going to target angle\n"));
		const float RotationChange = TargetAngleFromEventSpeed * DeltaTime;

		FRotator TargetRotation = TargetAngleFromEvent;
		TargetRotation.Roll = bTargetAngleOverrideRoll ? TargetRotation.Roll : OutViewRotation.Roll;
		TargetRotation.Pitch = bTargetAngleOverridePitch ? TargetRotation.Pitch : OutViewRotation.Pitch;
		TargetRotation.Yaw = bTargetAngleOverrideYaw ? TargetRotation.Yaw : OutViewRotation.Yaw;

		OutViewRotation = FMath::Lerp(OutViewRotation.Quaternion(), TargetRotation.Quaternion(), RotationChange).Rotator();


		const bool bHasArrived = (TargetRotation - OutViewRotation).IsNearlyZero(3.0f);

		if (bHasArrived == true)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Has Arrived\n"));
			GoingToTargetAngle = false;
			TargetAngleFromEvent = FRotator(0.0f);
			TargetAngleFromEventSpeed = 0.0f;
			bTargetAngleOverrideRoll = false;
			bTargetAngleOverridePitch = false;
			bTargetAngleOverrideYaw = false;
		}
	}
	// TODO: Should not autorotate when colliding with obstacles
	// Automatic alignment if no input is received from the player in align delay time
	/*
	else if (GetWorld()->TimeSeconds - LastManualRotationTime < AlignDelay == false)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Automatic Alignment\n"));
		FVector2D Movement = FVector2D(
			FocusLocation.Y - PreviousFocusLocation.Y,
			FocusLocation.X - PreviousFocusLocation.X);

		float MovementDeltaSqr = Movement.SizeSquared();
		//UE_LOG(LogTemp, Warning, TEXT("MovementDeltaSqr: %f\n"), MovementDeltaSqr);
		
		if (MovementDeltaSqr < 0.0001f == false)
		{
			float HeadingAngle = GetAngleFrom2DDirection(Movement / FMath::Sqrt(MovementDeltaSqr));
			
			FVector2D MoveVectorDirection = Movement;
			MoveVectorDirection.Normalize();
			UE_LOG(LogTemp, Warning, TEXT("Movement Vector: %f, %f\n"), MoveVectorDirection.X, MoveVectorDirection.Y);
			UE_LOG(LogTemp, Warning, TEXT("Heading Angle: %f\n"), HeadingAngle);
			OutViewRotation.Yaw = HeadingAngle;
			//UE_LOG(LogTemp, Warning, TEXT("Delta Rot: %f\n"), OutDeltaRot.Yaw);
			//float RotationChange = AutomaticRotationSpeed * DeltaTime;

			//OutViewRotation.Yaw = FMath::Lerp(OutViewRotation.Yaw, HeadingAngle, RotationChange);
		}
	}
	*/
	/*
	const FVector TargetForward = ViewTarget.Target->GetActorForwardVector();
	FVector2D Forward2D;
	Forward2D.X = TargetForward.Y;
	Forward2D.Y = TargetForward.X;
	float HeadingAngle = GetAngleFrom2DDirection(Forward2D);
	UE_LOG(LogTemp, Warning, TEXT("Heading Angle: %f\n"), HeadingAngle);
	*/

	/*
	FVector MoveVector = FocusLocation - PreviousFocusLocation;
	MoveVector.Normalize();
	const FVector EndPoint = PreviousFocusLocation + MoveVector * 100.0f;
	DrawDebugLine(GetWorld(), PreviousFocusLocation, EndPoint, FColor::Cyan);
	*/

	/*
	float headingAngle = GetAngle(movement / Mathf.Sqrt(movementDeltaSqr));
	float deltaAbs = Mathf.Abs(Mathf.DeltaAngle(orbitAngles.y, headingAngle));
	float rotationChange =
		rotationSpeed * Mathf.Min(Time.unscaledDeltaTime, movementDeltaSqr);
	if (deltaAbs < alignSmoothRange) {
		rotationChange *= deltaAbs / alignSmoothRange;
	}
	else if (180f - deltaAbs < alignSmoothRange) {
		rotationChange *= (180f - deltaAbs) / alignSmoothRange;
	}
	orbitAngles.y =
		Mathf.MoveTowardsAngle(orbitAngles.y, headingAngle, rotationChange);
		*/
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
	Super::UpdateViewTargetInternal(OutVT, DeltaTime);

	FVector CameraLocation;
	FRotator CameraRotation;
	GetCameraViewPoint(CameraLocation, CameraRotation);

	FVector CameraForward = CameraRotation.Vector();
	CameraForward.Z = 0.0f;
	CameraForward.Normalize();

	FVector CameraRight = -FVector::CrossProduct(CameraForward, FVector::UpVector);
	CameraRight.Z = 0.0f;
	CameraRight.Normalize();

	FVector FTargetLocation = OutVT.Target->GetActorLocation();
	FVector TargetToFocus = FocusLocation - FTargetLocation;
	float TargetToFocusDistance = TargetToFocus.Size();
	TargetToFocus.Normalize();

	FVector2D Projection;
	Projection.X = FVector::DotProduct(TargetToFocus, CameraForward);
	Projection.Y = FVector::DotProduct(TargetToFocus, CameraRight);
	Projection.Normalize();

	const FVector2D RelativeDistance = -Projection * TargetToFocusDistance;

	//UE_LOG(LogTemp, Warning, TEXT("Relative Distance: %f, %f\n"), RelativeDistance.X, RelativeDistance.Y);

	PreviousFocusLocation = FocusLocation;

	FVector SocketOffset = SpringArmComponent->SocketOffset;
	FTViewTarget TargetView = OutVT;
	
	const FVector TargetLocation = TargetView.Target->GetActorLocation();
	
	if (HorizontalFocusRadius > 0.0f)
	{
		/*
		float t = 1.0f;
		if (FMath::Abs(RelativeDistance.Y) > 0.01f && FocusCentering > 0.0f)
		{
			t = FMath::Pow(1 - FocusCentering, DeltaTime);
		}

		if (FMath::Abs(RelativeDistance.Y) > HorizontalFocusRadius)
		{
			t = FMath::Min(t, HorizontalFocusRadius / FMath::Abs(RelativeDistance.Y));
		}
		*/

		FVector Delta = TargetLocation - FocusLocation;
		Delta.Z = 0.0f;
		Delta = -Delta;

		//UKismetSystemLibrary::DrawDebugLine(GetWorld(), TargetLocation, FocusLocation, FLinearColor::Green, 1.0f, 2.0f);

		float AmountForward = FVector::DotProduct(Delta, CameraForward);
		float AmountRight = FVector::DotProduct(Delta, CameraRight);

		// TODO: Make it quadratic
		AmountForward *= 0.3f;
		AmountRight *= 0.95f;

		SocketOffset.X = AmountForward;
		SocketOffset.Y = AmountRight;
		
		FocusLocation = TargetLocation + AmountRight * CameraRight + AmountForward * CameraForward;
		//FocusLocation = FMath::Lerp(TargetLocation, FocusLocation, t);
		//UKismetSystemLibrary::DrawDebugLine(GetWorld(), TargetLocation + CameraForward * 100.0f, TargetLocation + CameraForward * 100.0f + FVector::UpVector * 100.0f, FLinearColor::Blue, 1.0f, 2.0f);
		//UKismetSystemLibrary::DrawDebugLine(GetWorld(), TargetLocation + CameraRight * 100.0f, TargetLocation + CameraRight * 100.0f + FVector::UpVector * 100.0f, FLinearColor::Red, 1.0f, 2.0f);
		//UKismetSystemLibrary::DrawDebugLine(GetWorld(), FocusLocation, FocusLocation + FVector::UpVector * 100.0f, FLinearColor::Green, 1.0f, 2.0f);
		//UKismetSystemLibrary::DrawDebugLine(GetWorld(), PreviousFocusLocation, PreviousFocusLocation + FVector::UpVector * 100.0f, FLinearColor::Red, 1.0f, 2.0f);
		//SocketOffset.Y = -RelativeDistance.Y;
	}
	else
	{
		FocusLocation = TargetLocation;
	}

	SpringArmComponent->SocketOffset = SocketOffset;

	//UE_LOG(LogTemp, Warning, TEXT("Previous Focus Location: %f, %f, %f\n"), FocusLocation.X, FocusLocation.Y, FocusLocation.Z);
	//UE_LOG(LogTemp, Warning, TEXT("Previous Focus Location: %f, %f, %f\n"), PreviousFocusLocation.X, PreviousFocusLocation.Y, PreviousFocusLocation.Z);
}