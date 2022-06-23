// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_RoundGameMode.h"
#include "GameFramework/Actor.h"
#include "RGX_ScoreGameState.h"
#include "Engine/AssetManager.h"
#include "GameplayTagContainer.h"
#include "RegicideX/Data/RGX_EnemyDataAsset.h"
#include "Kismet/GameplayStatics.h"

TArray<AActor*> ARGX_RoundGameMode::GetEnemySpawners() const
{
	return EnemySpawners;
}

void ARGX_RoundGameMode::SetEnemySpawners(const TArray<AActor*>& EnemySpawnersList)
{
	this->EnemySpawners = EnemySpawnersList;
}

ARGX_RoundGameMode::ARGX_RoundGameMode()
{
	GameStateClass = ARGX_ScoreGameState::StaticClass();
	EnemySpawners.Init(nullptr, 0);
}

int ARGX_RoundGameMode::GetScore() const
{
	return GetGameState<ARGX_ScoreGameState>()->GetScore();
}

void ARGX_RoundGameMode::SetScore(const int NewScore)
{
	GetGameState<ARGX_ScoreGameState>()->SetScore(NewScore);
}

int ARGX_RoundGameMode::GetRound() const
{
	return GetGameState<ARGX_ScoreGameState>()->GetRound();
}

void ARGX_RoundGameMode::StartPlay()
{
	StartPlayEvent();
	TargetActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	StartEnemySpawn();
	Super::StartPlay(); //Must be at the end
}

void ARGX_RoundGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetGameState<ARGX_ScoreGameState>()->SetStateDefaults();
	StartPlay();
}

void ARGX_RoundGameMode::StartWaveEvent_Implementation()
{
}

void ARGX_RoundGameMode::EndWaveEvent_Implementation()
{
}

void ARGX_RoundGameMode::EndGameEvent_Implementation()
{
}

void ARGX_RoundGameMode::EnemyDeadEvent_Implementation()
{
}

void ARGX_RoundGameMode::StartEnemySpawn()
{
	if (TargetActor == nullptr)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		AActor* Target = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		if (Target)
			TargetActor = Target;
		else
		{
			UE_LOG(LogTemp, Error, TEXT("No player character available."));
			return;
		}
	}

	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARGX_EnemySpawner::StaticClass(), EnemySpawners);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARGX_PoolSpawner::StaticClass(), EnemySpawners);

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ARGX_RoundGameMode::StartNewWave, 4.0f, false);
	//StartNewWave();
}
void ARGX_RoundGameMode::IncreaseKillCount()
{
	KillCount++;
	SpawnedEnemies--;
	CheckCurrentWave();
}

void ARGX_RoundGameMode::StartPlayEvent_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StartPlay Default Called"));
	
}

void ARGX_RoundGameMode::StartNewWave()
{
	if (DTRounds == nullptr || DTEnemies == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Rounds or Enemies data tables available."))
			return;
	}

	FString WaveName = WaveName.FromInt(CurrentWave);
	WaveName = "Round" + WaveName;
	const FName FRoundName = FName(WaveName);

	FRGX_RoundDataTable* round = DTRounds->FindRow<FRGX_RoundDataTable>(FRoundName, "");

	if (round == nullptr) {
		EndGameEvent();
		return;
	}

	RoundInfo = *DTRounds->FindRow<FRGX_RoundDataTable>(FRoundName, "");

	EnemyWaveNames = DTEnemies->GetRowNames();

	StartWaveEvent();
	SpawnNewWave();
}

void ARGX_RoundGameMode::CheckCurrentWave()
{
	if (SpawnedEnemies <= 0)
	{
		// TODO OnWaveFinished --> print widget in the screen
		CurrentWave++;
		OnWaveFinished();
	}
	// TODO If not all enemies have spawned, spawn them
}

void ARGX_RoundGameMode::SpawnNewWave()
{
	// Show round widget

	SpawnEnemyGroups();
}

void ARGX_RoundGameMode::SpawnEnemyGroups()
{
	for (int i = 0; i < EnemyWaveNames.Num(); ++i)
	{
		for (int j = 0; j < RoundInfo.EnemiesToSpawn[i]; ++j)
		{
			SpawnEnemy(DTEnemies->FindRow<FRGX_EnemiesDataTable>(EnemyWaveNames[i], "")->EnemyInfo);
		}
	}
}

void ARGX_RoundGameMode::SpawnEnemy(UDataAsset* EnemyInfo)
{
	const URGX_EnemyDataAsset* EnemyInfoCasted = Cast<URGX_EnemyDataAsset>(EnemyInfo);

	if (EnemyInfoCasted->EnemyBP && EnemySpawners.Num() > 0)
	{
		const int Rand = FMath::RandRange(0, EnemySpawners.Num() - 1);
		if (EnemySpawners[Rand])
		{
			//if (ARGX_EnemyBase* Enemy = (Cast<ARGX_EnemySpawner>(EnemySpawners[Rand])->Spawn(EnemyInfoCasted->EnemyBP)))
			if (ARGX_EnemyBase* Enemy = (Cast<ARGX_PoolSpawner>(EnemySpawners[Rand])->Spawn(EnemyInfoCasted->TypeName)))
			{
				Enemy->OnHandleDeathEvent.AddUObject(this, &ARGX_RoundGameMode::OnEnemyDestroyed);
				Enemy->TargetActor = TargetActor;
				SpawnedEnemies++;
			}
		}

	}
}

void ARGX_RoundGameMode::OnEnemyDestroyed(const int EnemyScoreValue)
{
	IncreaseKillCount();
	ARGX_ScoreGameState* GameStateTemp = GetGameState<ARGX_ScoreGameState>();
	GameStateTemp->SetScore(GameStateTemp->GetScore() + EnemyScoreValue);
	EnemyDeadEvent();
}

void ARGX_RoundGameMode::OnWaveFinished()
{
	// Throw end event to print End of Wave widget.
	EndWaveEvent();

	ARGX_ScoreGameState* GameStateTemp = GetGameState<ARGX_ScoreGameState>();
	GameStateTemp->SetRound(CurrentWave);

	// Set a timer to delay the begining of the new wave.
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ARGX_RoundGameMode::StartNewWave, 4.0f, false);
	//GetGameState<ARGX_ScoreGameState>()
}
