// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_EnemySpawner.h"
#include "BehaviorTree/BehaviorTree.h"
#include "RGX_Peasant.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "NavigationSystem.h"

// Sets default values
ARGX_EnemySpawner::ARGX_EnemySpawner()
{

}

// Called when the game starts or when spawned
void ARGX_EnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnBox = SpawnBox.ShiftBy(GetActorLocation());

	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FVector Scale(0.3f);
}

ARGX_EnemyBase* ARGX_EnemySpawner::Spawn(const TSubclassOf<ARGX_EnemyBase> EnemyBP)
{
	// @todo: Make spawner smarter -> get parameters from Data Asset and preset actor parameter
	const FVector spawnerLocation = GetActorLocation();
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(this);

	const FRotator Rotation(0.0f, 0.0f, 0.0f);
	for (int32 i = 0; i < 10; ++i)
	{
		FNavLocation spawnLocation;
		if (NavSystem->GetRandomReachablePointInRadius(spawnerLocation, SpawnRadius, spawnLocation))
		{
			FVector finalLocation = spawnLocation.Location;
			finalLocation.Z = spawnerLocation.Z;

			if (ARGX_EnemyBase* SpawnedEnemy = GetWorld()->SpawnActor<ARGX_EnemyBase>(EnemyBP, finalLocation, Rotation))
			{
				return SpawnedEnemy;
			}
		}
	}

	return nullptr;
}
