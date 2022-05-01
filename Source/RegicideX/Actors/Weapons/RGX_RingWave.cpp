// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_RingWave.h"

// Sets default values
ARGX_RingWave::ARGX_RingWave()
	: AActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

// Called when the game starts or when spawned
void ARGX_RingWave::BeginPlay()
{
	Super::BeginPlay();

}

void ARGX_RingWave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARGX_RingWave::HitActor(UPrimitiveComponent* OverlappedComponent
	, AActor* OtherActor
	, UPrimitiveComponent* OtherComp
	, int32 OtherBodyIndex
	, bool bFromSweep
	, const FHitResult& SweepResult)
{
	
}
