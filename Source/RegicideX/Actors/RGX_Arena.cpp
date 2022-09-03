// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/Actors/RGX_Arena.h"

// Sets default values
ARGX_Arena::ARGX_Arena()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARGX_Arena::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARGX_Arena::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

