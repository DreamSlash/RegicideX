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

	SpawnBox = SpawnBox.ShiftBy(GetActorLocation());

	//Spawn();
	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FVector Scale(0.3f);

	//GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ARGX_EnemySpawner::Spawn, 4.0f, true);
}

ARGX_EnemyBase* ARGX_EnemySpawner::Spawn(TSubclassOf<ARGX_EnemyBase> EnemyBP)
{
	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Blue, TEXT("Spawning enemy"));

	const FVector Location = FMath::RandPointInBox(SpawnBox) + FVector(0.0f, 0.0f, 300.0f);
	const FRotator Rotation(0.0f, 0.0f, 0.0f);

	ARGX_EnemyBase* SpawnedEnemy = GetWorld()->SpawnActor<ARGX_EnemyBase>(EnemyBP, Location, Rotation);
	return SpawnedEnemy;

	/*
	if (Manager && Manager->bCanSpawn)
	{
		
	} else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("No manager available ..."));
	}
	*/

	//AActor* Actor = nullptr;
	/*
	if (Manager && Manager->bCanSpawn)
	{
		if (Manager->CurrentNumberOfPeasants < Manager->MaxNumberOfPeasants)
		{
			const int NToSpawn = Manager->MaxNumberOfPeasants - Manager->CurrentNumberOfPeasants;
			//FVector SpawnerLocation = GetActorLocation();
			for (int i = 0; i < NToSpawn; ++i)
			{
				FVector Location = FMath::RandPointInBox(SpawnBox) + FVector(0.0f, 0.0f, 90.0f);
				FRotator Rotation(0.0f, 0.0f, 0.0f);
				//const FTransform Transform(Rotation, Location);

				ARGX_EnemyBase* SpawnedEnemy = GetWorld()->SpawnActor<ARGX_EnemyBase>(EnemyBP, Location, Rotation);
				
				//SpawnPeasant(Transform);
			}
			Manager->OnPeasantAdded();
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("No manager available ..."));
	}*/
}
