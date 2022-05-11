// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_MageAngel.h"

ARGX_MageAngel::ARGX_MageAngel()
	: ARGX_EnemyBase()
{
	RingWaveSource = CreateDefaultSubobject<USceneComponent>(TEXT("RingWaveSource"));
	RingWaveSource->SetupAttachment(RootComponent);
}

void ARGX_MageAngel::BeginPlay()
{
	Super::BeginPlay();

	ForceHeight();
}

void ARGX_MageAngel::MoveToTarget(float DeltaTime, FVector TargetPos)
{
	Super::MoveToTarget(DeltaTime, TargetPos);

	ForceHeight();
}

void ARGX_MageAngel::ForceHeight()
{
	FVector NewLocation = GetActorLocation();
	NewLocation.Z = HeightPos;
	SetActorLocation(NewLocation);
}