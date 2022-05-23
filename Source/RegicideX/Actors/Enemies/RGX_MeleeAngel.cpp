// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_MeleeAngel.h"

ARGX_MeleeAngel::ARGX_MeleeAngel()
{

}

void ARGX_MeleeAngel::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	RotateToTarget(DeltaTime);

}