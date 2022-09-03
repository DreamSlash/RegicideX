// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/Actors/RGX_Arena.h"
#include "Components/BoxComponent.h"
#include "RegicideX/Actors/Enemies/RGX_EnemySpawner.h"
#include "RegicideX/Character/RGX_PlayerCharacter.h"
#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "Kismet/GameplayStatics.h"

ARGX_Arena::ARGX_Arena()
{
	PrimaryActorTick.bCanEverTick = true;

	ArenaArea = CreateDefaultSubobject<UBoxComponent>(TEXT("ArenaArea"));
}

void ARGX_Arena::BeginPlay()
{
	Super::BeginPlay();
	InitializeSpawners();

	PlayerCharacter = Cast<ARGX_PlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	ArenaArea->OnComponentBeginOverlap.AddDynamic(this, &ARGX_Arena::OnComponentBeginOverlap);
	ArenaArea->OnComponentEndOverlap.AddDynamic(this, &ARGX_Arena::OnComponentEndOverlap);
}

void ARGX_Arena::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	ArenaArea->OnComponentBeginOverlap.RemoveDynamic(this, &ARGX_Arena::OnComponentBeginOverlap);
	ArenaArea->OnComponentEndOverlap.RemoveDynamic(this, &ARGX_Arena::OnComponentEndOverlap);
}


void ARGX_Arena::InitializeSpawners()
{
	TSet<AActor*> OverlappingActors;
	ArenaArea->GetOverlappingActors(OverlappingActors, EnemySpawnerClass);
	
	for (AActor* Actor : OverlappingActors)
	{
		ARGX_EnemySpawner* EnemySpawner = Cast<ARGX_EnemySpawner>(Actor);
		EnemySpawners.Add(EnemySpawner);
	}

	if (EnemySpawners.Num() > 0)
	{
		bIsInitialized = true;
	}
}

void ARGX_Arena::SpawnWave()
{
	if (EnemySpawners.Num() <= 0) return;
	
	for (int i = 0; i < NumEnemiesToSpawn; i++)
	{
		const int Rand = FMath::RandRange(0, EnemySpawners.Num() - 1);
		SpawnEnemy(Rand);
	}

	bEnemiesSpawned = true;
}

void ARGX_Arena::SpawnEnemy(int32 SpawnerNum)
{
	if (EnemySpawners[SpawnerNum])
	{
		if (ARGX_EnemyBase* Enemy = (Cast<ARGX_EnemySpawner>(EnemySpawners[SpawnerNum])->Spawn(EnemyClass)))
		{
			Enemy->OnHandleDeathEvent.AddUObject(this, &ARGX_Arena::OnEnemyDeath);
			Enemy->TargetActor = PlayerCharacter;
			EnemiesLeft++;
		}
	}
}

void ARGX_Arena::HandleFinishWave()
{
	UE_LOG(LogTemp, Warning, TEXT("Wave Finished"));
}

void ARGX_Arena::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARGX_PlayerCharacter* Player = Cast<ARGX_PlayerCharacter>(OtherActor);
	if (Player)
	{
		bActivated = true;
		if (OnArenaActivated.IsBound())
		{
			OnArenaActivated.Broadcast(this);
		}

		UE_LOG(LogTemp, Warning, TEXT("Player Begin Overlap"));
	}
}

void ARGX_Arena::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ARGX_PlayerCharacter* Player = Cast<ARGX_PlayerCharacter>(OtherActor);
	if (Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player End Overlap"));
	}
}

void ARGX_Arena::OnEnemyDeath(int32 Score)
{
	UE_LOG(LogTemp, Warning, TEXT("On Enemy Death"));
	EnemiesLeft--;
	if (EnemiesLeft == 0)
	{
		HandleFinishWave();
	}
}

void ARGX_Arena::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Overlaps do not work until iteration of overlaps
	if (bIsInitialized == false)
	{
		// Get spawners in area
		InitializeSpawners();
	}

	if (bActivated == false || bFinished == true) return;

	if (bEnemiesSpawned == true) return;

	// Spawn Wave
	SpawnWave();
}

