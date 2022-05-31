// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/Character/RGX_PlayerCharacterController.h"
#include "RegicideX/RGX_PlayerCameraManager.h"

void ARGX_PlayerCharacterController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("Player Tick\n"));
}

void ARGX_PlayerCharacterController::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PreProcessInput(DeltaTime, bGamePaused);
	//UE_LOG(LogTemp, Warning, TEXT("PreProcessInput\n"));
}

void ARGX_PlayerCharacterController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PostProcessInput(DeltaTime, bGamePaused);
	//UE_LOG(LogTemp, Warning, TEXT("PostProcessInput\n"));
}

void ARGX_PlayerCharacterController::OnStartFallAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("OnStartFallAttack\n"));
	ARGX_PlayerCameraManager* RGX_PlayerCameraManager = Cast<ARGX_PlayerCameraManager>(PlayerCameraManager);

	// TODO [WARNING]: Do not put a value impossible to rotate to because of constraints
	FRotator TargetRotation;
	TargetRotation.Roll = 0.0f;
	TargetRotation.Pitch = -60.0f;
	TargetRotation.Yaw = 0.0f;
	RGX_PlayerCameraManager->SetTargetAngle(TargetRotation, 20.0f, false, true, false);
}

void ARGX_PlayerCharacterController::OnEndFallAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("OnEndFallAttack\n"));
}
