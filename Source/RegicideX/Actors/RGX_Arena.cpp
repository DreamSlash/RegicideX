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
	//InitializeSpawners();

	PlayerCharacter = Cast<ARGX_PlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	ArenaArea->OnComponentBeginOverlap.AddDynamic(this, &ARGX_Arena::OnComponentBeginOverlap);
	ArenaArea->OnComponentEndOverlap.AddDynamic(this, &ARGX_Arena::OnComponentEndOverlap);

	for (int i = 0; i < InitialWavesDataAssets.Num(); i++)
	{
		URGX_OutgoingWave* CurrentWave = NewObject<URGX_OutgoingWave>(this, URGX_OutgoingWave::StaticClass());
		if (CurrentWave)
		{
			CurrentWave->WaveData = InitialWavesDataAssets[i];
			CurrentWave->OnWaveFinished.AddDynamic(this, &ARGX_Arena::OnHandleFinishWave);
			CurrentWaves.Add(CurrentWave);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create OutgoingWave class"));
		}
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
	// TODO: GetOverlappingActors does not return the correct result, just the first Spawner
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

void ARGX_Arena::SpawnInitialWaves()
{
	for (int i = 0; i < CurrentWaves.Num(); i++)
	{
		SpawnWave(CurrentWaves[i]);
	}

	for (int j = 0; j < InitialConstantPeasants; j++)
	{
		SpawnConstantPeasant();
	}

	bInitialWavesSpawned = true;
}

void ARGX_Arena::SpawnWave(URGX_OutgoingWave* Wave)
{
	const float SpawnDelay = Wave->WaveData->SpawnTimeDelay;

	if (SpawnDelay > 0.1f)
	{
		FTimerDelegate TimerDel;
		FTimerHandle TimerHandle;
		TimerDel.BindUFunction(this, FName("HandleSpawnWave"), Wave);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, SpawnDelay, false);
	}
	else
	{
		HandleSpawnWave(Wave);
	}
}

void ARGX_Arena::HandleSpawnWave(URGX_OutgoingWave* Wave)
{
	if (EnemySpawners.Num() <= 0) return;

	TArray<FName> EnemyWaveNames = DT_EnemyRefs->GetRowNames();
	URGX_ArenaWaveDataAsset* CurrentWaveData = Wave->WaveData;

	if (EnemyWaveNames.Num() != CurrentWaveData->NumEnemies.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("WaveData num enemies different from total number of enemy types available"));
		return;
	}

	LastSpawnerIdx = -1;
	for (int i = 0; i < CurrentWaveData->NumEnemies.Num(); ++i)
	{
		SpawnWaveEnemyTypeGroup(EnemyWaveNames[i], CurrentWaveData->NumEnemies[i], Wave);
	}

	Wave->bEnemiesSpawned = true;
}

void ARGX_Arena::SpawnWaveEnemyRandomMode(TSubclassOf<class ARGX_EnemyBase> EnemyClass, URGX_OutgoingWave* Wave)
{
	int SpawnerIdx = 0;
	TSet<int>& AvailableSpawnersIdx = Wave->WaveData->SpawnerIdxAvailable;

	if (AvailableSpawnersIdx.Num() == 0)
	{
		SpawnerIdx = FMath::RandRange(0, EnemySpawners.Num() - 1);
	}
	else
	{
		do
		{
			SpawnerIdx = FMath::RandRange(0, EnemySpawners.Num() - 1);
		} while (AvailableSpawnersIdx.Contains(SpawnerIdx) == false);
	}

	SpawnWaveEnemy(EnemyClass, SpawnerIdx, Wave);
}

void ARGX_Arena::SpawnWaveEnemyRoundRobinMode(TSubclassOf<class ARGX_EnemyBase> EnemyClass, URGX_OutgoingWave* Wave)
{
	int SpawnerIdx = 0;
	TSet<int>& AvailableSpawnersIdx = Wave->WaveData->SpawnerIdxAvailable;

	if (AvailableSpawnersIdx.Num() == 0)
	{
		SpawnerIdx = Wave->EnemiesLeft % EnemySpawners.Num();
	}
	else if (AvailableSpawnersIdx.Num() == 1)
	{	
		// TODO: Guarreiro pero no em se la sintaxi per pillar l'unic element del set
		for (int i : AvailableSpawnersIdx)
		{
			SpawnerIdx = i;
			break;
		}
	}
	else
	{
		// TODO: Bug. Incorrect idx sometimes
		SpawnerIdx = Wave->EnemiesLeft % EnemySpawners.Num();
		while (SpawnerIdx == LastSpawnerIdx || AvailableSpawnersIdx.Contains(SpawnerIdx) == false)
		{
			if (SpawnerIdx + 1 == EnemySpawners.Num())
			{
				SpawnerIdx = 0;
			}
			else
			{
				SpawnerIdx++;
			}
		}

		LastSpawnerIdx = SpawnerIdx;
	}

	SpawnWaveEnemy(EnemyClass, SpawnerIdx, Wave);
}

// TODO: Petar-se lu de EnemyWaveName. Amb idx ja es pot accedir a la info d'un enemic
void ARGX_Arena::SpawnWaveEnemyTypeGroup(const FName& EnemyWaveName, int32 NumEnemies, URGX_OutgoingWave* Wave)
{
	for (int j = 0; j < NumEnemies; ++j)
	{
		UDataAsset* EnemyInfo = DT_EnemyRefs->FindRow<FRGX_EnemiesDataTable>(EnemyWaveName, "")->EnemyInfo;
		const URGX_EnemyDataAsset* EnemyInfoCasted = Cast<URGX_EnemyDataAsset>(EnemyInfo);
		if (EnemyInfoCasted && EnemyInfoCasted->EnemyBP)
		{
			switch(Wave->WaveData->WaveSpawnMode)
			{
			case ERGX_WaveSpawnMode::Random:
			{
				SpawnWaveEnemyRandomMode(EnemyInfoCasted->EnemyBP, Wave);
				break;
			}
			case ERGX_WaveSpawnMode::RoundRobin:
			{
				SpawnWaveEnemyRoundRobinMode(EnemyInfoCasted->EnemyBP, Wave);
				break;
			}
		
			default: // Default is random spawn
			{
				SpawnWaveEnemyRandomMode(EnemyInfoCasted->EnemyBP, Wave);
				break;
			}
			}
		}
	}
}

void ARGX_Arena::SpawnWaveEnemy(TSubclassOf<ARGX_EnemyBase> EnemyClass, int32 SpawnerIdx, URGX_OutgoingWave* Wave)
{
	if (EnemySpawners[SpawnerIdx])
	{
		if (ARGX_EnemyBase* Enemy = (Cast<ARGX_EnemySpawner>(EnemySpawners[SpawnerIdx])->Spawn(EnemyClass)))
		{
			Enemy->OnHandleDeathEvent.AddDynamic(this, &ARGX_Arena::OnEnemyDeath);
			Enemy->OnHandleDeathEvent.AddDynamic(Wave, &URGX_OutgoingWave::OnEnemyDeath);
			Enemy->TargetActor = PlayerCharacter;
			Wave->EnemiesLeft++;
			EnemiesLeft++;
		}
	}
}

void ARGX_Arena::SpawnConstantPeasant()
{
	if (PeasantClass.Get() == nullptr) return;

	const int SpawnerIdx = FMath::RandRange(0, EnemySpawners.Num() - 1);
	if (EnemySpawners[SpawnerIdx])
	{
		if (ARGX_EnemyBase* Enemy = (Cast<ARGX_EnemySpawner>(EnemySpawners[SpawnerIdx])->Spawn(PeasantClass)))
		{
			Enemy->OnHandleDeathEvent.AddDynamic(this, &ARGX_Arena::OnConstantPeasantDeath);
			Enemy->TargetActor = PlayerCharacter;
			CurrentNumberConstantPeasant++;
			EnemiesLeft++;
		}
	}
}

void ARGX_Arena::OnHandleFinishWave(URGX_OutgoingWave* FinishedWave)
{
	UE_LOG(LogTemp, Warning, TEXT("Wave Finished"));

	FTimerDelegate TimerDel;
	FTimerHandle TimerHandle;
	TimerDel.BindUFunction(this, FName("HandleFinishWave"), FinishedWave);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, TimeBetweenWaves, false);
}

void ARGX_Arena::HandleFinishWave(URGX_OutgoingWave* FinishedWave)
{
	URGX_ArenaWaveDataAsset* CurrentWaveData = FinishedWave->WaveData;

	if (CurrentWaveData->ChildWave)
	{
		// Reuse wave
		FinishedWave->WaveData = CurrentWaveData->ChildWave;
		FinishedWave->EnemiesLeft = 0;
		FinishedWave->bEnemiesSpawned = false;
		SpawnWave(FinishedWave);
	}
	else
	{
		CurrentWaves.Remove(FinishedWave);
		if (CurrentWaves.Num() == 0 && EnemiesLeft == 0)
		{
			HandleFinishArena();
		}
	}
}

void ARGX_Arena::HandleFinishArena()
{
	if (bFinished == true) return;

	UE_LOG(LogTemp, Warning, TEXT("Arena Finished"));

	bFinished = true;
	bActivated = false;

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
		/*
		bActivated = true;
		if (OnArenaActivated.IsBound())
		{
			OnArenaActivated.Broadcast(this);
		}
		*/
		//UE_LOG(LogTemp, Warning, TEXT("Player Begin Overlap"));
	}
}

void ARGX_Arena::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ARGX_PlayerCharacter* Player = Cast<ARGX_PlayerCharacter>(OtherActor);
	if (Player)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Player End Overlap"));
	}
}

void ARGX_Arena::OnEnemyDeath(ARGX_EnemyBase* Enemy)
{
	UE_LOG(LogTemp, Warning, TEXT("Arena On Enemy Death"));
	EnemiesLeft--;

	if (OnArenaEnemyKilled.IsBound())
	{
		OnArenaEnemyKilled.Broadcast(Enemy);
	}

	if (CurrentWaves.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Waves = 0"));
	}
	else if (CurrentWaves[0] == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Manuela"));
	}
}

void ARGX_Arena::OnConstantPeasantDeath(ARGX_EnemyBase* Enemy)
{
	EnemiesLeft--;
	CurrentNumberConstantPeasant--;

	if (OnArenaEnemyKilled.IsBound())
	{
		OnArenaEnemyKilled.Broadcast(Enemy);
	}

	if (EnemiesLeft == 0 && CurrentWaves.Num() == 0)
	{
		HandleFinishArena();
	}
}

void ARGX_Arena::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bActivated == false || bFinished == true) return;

	if (bInitialWavesSpawned != true)
	{
		SpawnInitialWaves();
	}

	// Do not spawn constant peasants if the waves are finished
	if (CurrentNumberConstantPeasant < MaxNumConstantPeasants && CurrentWaves.Num() > 0)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			const float CurrentTime = World->GetTimeSeconds();
			if (CurrentTime - ConstantPeasantLastSpawnTime > ConstantPeasantSpawnRate)
			{
				SpawnConstantPeasant();
				ConstantPeasantLastSpawnTime = CurrentTime;
			}
		}
	}
}

void ARGX_Arena::ActivateArena()
{
	if (bActivated == true) return;

	bActivated = true;
	if (OnArenaActivated.IsBound())
	{
		OnArenaActivated.Broadcast(this);
	}
}

void ARGX_Arena::DeactivateArena()
{
	if (bActivated == false) return;

	HandleFinishArena();
}

void URGX_OutgoingWave::OnEnemyDeath(ARGX_EnemyBase* Enemy)
{
	EnemiesLeft--;

	UE_LOG(LogTemp, Log, TEXT("OutgoingWave OnEnemyDeath. Remaining Wave Enemies: %d"), EnemiesLeft);

	if (EnemiesLeft == 0 && bEnemiesSpawned == true)
	{
		if (OnWaveFinished.IsBound())
		{
			OnWaveFinished.Broadcast(this);
		}
	}
}