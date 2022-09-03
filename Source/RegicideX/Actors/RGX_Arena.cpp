// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/Actors/RGX_Arena.h"
#include "Components/BoxComponent.h"
#include "RegicideX/Actors/Enemies/RGX_EnemySpawner.h"
#include "RegicideX/Character/RGX_PlayerCharacter.h"
#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "RegicideX/Data/RGX_RoundDataTable.h"
#include "RegicideX/Data/RGX_EnemyDataAsset.h"

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

	CurrentWave = NewObject<URGX_OutgoingWave>(this, URGX_OutgoingWave::StaticClass());
	if (CurrentWave)
	{
		CurrentWave->WaveData = WaveDataAsset;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Failed to create OutgoingWave class"));
	}
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

	TArray<FName> EnemyWaveNames = DT_EnemyRefs->GetRowNames();
	URGX_ArenaWaveDataAsset* CurrentWaveData = CurrentWave->WaveData;

	if (EnemyWaveNames.Num() != CurrentWaveData->NumEnemies.Num()) return;

	for (int i = 0; i < CurrentWaveData->NumEnemies.Num(); ++i)
	{
		SpawnEnemyTypeGroup(EnemyWaveNames[i], CurrentWaveData->NumEnemies[i]);
	}

	bEnemiesSpawned = true;
}

// TODO: Petar-se lu de EnemyWaveName. Amb idx ja es pot accedir a la info d'un enemic
void ARGX_Arena::SpawnEnemyTypeGroup(const FName& EnemyWaveName, int32 NumEnemies)
{
	for (int j = 0; j < NumEnemies; ++j)
	{
		UDataAsset* EnemyInfo = DT_EnemyRefs->FindRow<FRGX_EnemiesDataTable>(EnemyWaveName, "")->EnemyInfo;
		const URGX_EnemyDataAsset* EnemyInfoCasted = Cast<URGX_EnemyDataAsset>(EnemyInfo);
		if (EnemyInfoCasted)
		{
			if (EnemyInfoCasted->EnemyBP)
			{
				const int SpawnerIdx = FMath::RandRange(0, EnemySpawners.Num() - 1);
				SpawnEnemy(EnemyInfoCasted->EnemyBP, SpawnerIdx);
			}
		}
	}
}

void ARGX_Arena::SpawnEnemy(TSubclassOf<ARGX_EnemyBase> EnemyClass, int32 SpawnerNum)
{
	if (EnemySpawners[SpawnerNum])
	{
		if (ARGX_EnemyBase* Enemy = (Cast<ARGX_EnemySpawner>(EnemySpawners[SpawnerNum])->Spawn(EnemyClass)))
		{
			Enemy->OnHandleDeathEvent.AddUObject(this, &ARGX_Arena::OnEnemyDeath);
			Enemy->OnHandleDeathEvent.AddUObject(CurrentWave, &URGX_OutgoingWave::OnEnemyDeath);
			Enemy->TargetActor = PlayerCharacter;
			CurrentWave->EnemiesLeft++;
			EnemiesLeft++;
		}
	}
}

void ARGX_Arena::HandleFinishWave()
{
	UE_LOG(LogTemp, Warning, TEXT("Wave Finished"));

	URGX_ArenaWaveDataAsset* CurrentWaveData = CurrentWave->WaveData;

	if (CurrentWaveData->ChildWaves.Num() > 0)
	{
		for (int i = 0; i < CurrentWaveData->ChildWaves.Num(); i++)
		{
			CurrentWave->WaveData = CurrentWaveData->ChildWaves[i];
			bEnemiesSpawned = false;
		}
	}
	else
	{
		HandleFinishArena();
	}
}

void ARGX_Arena::HandleFinishArena()
{
	UE_LOG(LogTemp, Warning, TEXT("Arena Finished"));

	bFinished = true;

	if (OnArenaDeactivated.IsBound())
	{
		OnArenaDeactivated.Broadcast(this);
	}
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

void URGX_OutgoingWave::OnEnemyDeath(int32 Score)
{
	UE_LOG(LogTemp, Log, TEXT("OutgoingWave OnEnemyDeath"));

	EnemiesLeft--;

	// If enemies are 0, trigger event
}
