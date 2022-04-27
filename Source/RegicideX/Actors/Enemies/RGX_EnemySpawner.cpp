// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BehaviorTree.h"
#include "RGX_EnemySpawner.h"
#include "RGX_GroupManager.h"
#include "RGX_Peasant.h"
#include "Kismet/GameplayStatics.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"

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

	FActorSpawnParameters SpawnInfo;
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ARGX_EnemySpawner::Spawn, 4.0f, true);
}

void ARGX_EnemySpawner::Spawn()
{
	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Blue, TEXT("Spawning enemies ..."));
	AActor* actor = nullptr;;
	if (manager)
	{
		if (manager->CurrentNumberOfPeasants < manager->MaxNumberOfPeasants)
		{
			FVector Location(0.0f, 0.0f, 90.0f);
			FRotator Rotation(0.0f, 0.0f, 0.0f);
			FTransform Transform(Rotation, Location);
			SpawnPeasant(Transform);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("No manager available ..."));
	}
}
