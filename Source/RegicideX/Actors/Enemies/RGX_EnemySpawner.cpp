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

	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FVector Scale(0.3f);
}

ARGX_EnemyBase* ARGX_EnemySpawner::Spawn(TSubclassOf<ARGX_EnemyBase> EnemyBP)
{
	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Blue, TEXT("Spawning enemy"));

	const FVector Location = FMath::RandPointInBox(SpawnBox) + FVector(150.0f, 150.0f, 300.0f);
	const FRotator Rotation(0.0f, 0.0f, 0.0f);

	//ARGX_Peasant* Peasant = Cast<ARGX_Peasant>(EnemyBP);
	//if (Peasant)
	//	Manager->AddPeasant(Peasant);

	ARGX_EnemyBase* SpawnedEnemy = GetWorld()->SpawnActor<ARGX_EnemyBase>(EnemyBP, Location, Rotation);
	return SpawnedEnemy;
}
