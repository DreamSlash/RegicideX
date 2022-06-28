// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_MageAngel.h"

#include "GameFramework/CharacterMovementComponent.h"

ARGX_MageAngel::ARGX_MageAngel()
	: ARGX_EnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	RingWaveSource = CreateDefaultSubobject<USceneComponent>(TEXT("RingWaveSource"));
	RingWaveSource->SetupAttachment(RootComponent);
	SetActorEnableCollision(true);
}

void ARGX_MageAngel::BeginPlay()
{
	Super::BeginPlay();

	// Set the height of the angel at the beginning
	FVector NewLocation = GetActorLocation();
	NewLocation.Z = HeightPos;
	SetActorLocation(NewLocation);

	// Remove gravity from Character Movement Component
	//GetCharacterMovement()->GravityScale = 0.0f;
}

void ARGX_MageAngel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bToBeDestroyed)
		Destroy();
}