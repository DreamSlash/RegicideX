// Fill out your copyright notice in the Description page of Project Settings.

#include "RGX_MageAngelController.h"

//#pragma optimize("", off)

void ARGX_MageAngelController::BeginPlay()
{
	Super::BeginPlay();
}

void ARGX_MageAngelController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	StartLogic();
}