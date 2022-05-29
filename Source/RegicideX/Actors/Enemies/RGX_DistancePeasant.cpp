// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_DistancePeasant.h"
#include "EngineUtils.h"

ARGX_DistancePeasant::ARGX_DistancePeasant()
{
	ClusterSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ClusterSpawnPoint"));
	ClusterSpawnPoint->SetupAttachment(RootComponent);
	ClusterSpawnPoint->SetRelativeLocation(FVector(100.0f, 0.0f, 90.0f));
}

void ARGX_DistancePeasant::BeginPlay()
{
	Super::BeginPlay();
}

void ARGX_DistancePeasant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
