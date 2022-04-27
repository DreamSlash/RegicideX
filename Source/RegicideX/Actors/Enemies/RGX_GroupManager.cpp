// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_GroupManager.h"
#include "RGX_Peasant.h"

// Sets default values
ARGX_GroupManager::ARGX_GroupManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PeasantArray.AddUninitialized(MaxNumberOfPeasants);
}

// Called when the game starts or when spawned
void ARGX_GroupManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARGX_GroupManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARGX_GroupManager::AddPeasant(ARGX_Peasant* newPeasant)
{
	if (!newPeasant)
		return;

	if (PeasantArray.Num() == MaxNumberOfPeasants) {
		// ERROR! Array is full, should not add a peasant when full.
		UE_LOG(LogTemp, Error, TEXT("Cannot add a new peasant to the manager if full.\n"));
		return;
	}

	for (int idx = 0; idx < PeasantArray.Num(); ++idx) {
		if (PeasantArray.IsValidIndex(idx))
			continue;
		
		PeasantArray[idx] = newPeasant;
		break;
	}

	newPeasant->bInCombat = true;

	CurrentNumberOfPeasants++;
}

void ARGX_GroupManager::RemovePeasant(ARGX_Peasant* PeasantToRemove)
{
	PeasantArray.RemoveSingle(PeasantToRemove);
	CurrentNumberOfPeasants--;
}

