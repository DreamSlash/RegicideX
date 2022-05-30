// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/Character/RGX_PlayerCharacterController.h"

void ARGX_PlayerCharacterController::PlayerTick(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("Player Tick\n"));
}

void ARGX_PlayerCharacterController::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
	UE_LOG(LogTemp, Warning, TEXT("PreProcessInput\n"));
}

void ARGX_PlayerCharacterController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	UE_LOG(LogTemp, Warning, TEXT("PostProcessInput\n"));
}

void ARGX_PlayerCharacterController::OnStartFallAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("OnStartFallAttack\n"));
}

void ARGX_PlayerCharacterController::OnEndFallAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("OnEndFallAttack\n"));
}
