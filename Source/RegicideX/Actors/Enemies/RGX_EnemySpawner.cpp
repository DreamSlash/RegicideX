// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_EnemySpawner.h"
#include "BehaviorTree/BehaviorTree.h"
#include "RGX_GroupManager.h"
#include "RGX_Peasant.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ARGX_EnemySpawner::ARGX_EnemySpawner()
{

}

// Called when the game starts or when spawned
void ARGX_EnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	Spawn();
	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FVector Scale(0.3f);

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ARGX_EnemySpawner::Spawn, 4.0f, true);
}

void ARGX_EnemySpawner::Spawn()
{
	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Blue, TEXT("Spawning enemies ..."));
	AActor* Actor = nullptr;
	if (manager && manager->bCanSpawn)
	{
		if (manager->CurrentNumberOfPeasants < manager->MaxNumberOfPeasants)
		{
			const int NToSpawn = manager->MaxNumberOfPeasants - manager->CurrentNumberOfPeasants;
			FVector SpawnerLocation = GetActorLocation();
			for (int i = 0; i < NToSpawn; ++i)
			{
				FVector Location = FMath::RandPointInBox(SpawnBox) + FVector(0.0f, 0.0f, 90.0f);
				FRotator Rotation(0.0f, 0.0f, 0.0f);
				const FTransform Transform(Rotation, Location);
				
				SpawnPeasant(Transform);
			}
			manager->OnPeasantAdded();
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("No manager available ..."));
	}
}
