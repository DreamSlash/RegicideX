// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_RoundGameMode.h"
#include "GameFramework/Actor.h"
#include "RGX_ScoreGameState.h"
#include "Engine/AssetManager.h"
#include "GameplayTagContainer.h"
#include "RegicideX/Data/RGX_EnemyDataAsset.h"
#include "RegicideX/Data/RGX_RoundDataTable.h"
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
	Super::StartPlay(); //Must be at the end
}

void ARGX_RoundGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetGameState<ARGX_ScoreGameState>()->SetStateDefaults();
	PopulateSpawnerList();
	// @todo: call enter cinematic
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ARGX_RoundGameMode::StartGameSpawn, 4.0f, false);
}

void ARGX_RoundGameMode::OnEnemyDeath(const int Type)
{
	// @todo: Investigate delegate to avoid explicit call

	ARGX_ScoreGameState* State = GetGameState<ARGX_ScoreGameState>();
	State->OnEnemyDeath(Type);
	if(State->GetNumEnemies() == 0) //@todo Rename NumEnemies
	{
		StartNextRound();
	}
}

void ARGX_RoundGameMode::StartNextRound()
{

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Next Round"));
	ARGX_ScoreGameState* GameStateTemp = GetGameState<ARGX_ScoreGameState>(); 
	GameStateTemp->StartNextRound();

	// Spawn enemies 4 seconds after round started.
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ARGX_RoundGameMode::SpawnEnemies, 4.0f, false);
}

void ARGX_RoundGameMode::SpawnEnemies()
{
	int SpawnedEnemies = 0;
	ARGX_ScoreGameState* GameStateTemp = GetGameState<ARGX_ScoreGameState>();
	if(DTRounds == nullptr || DTEnemies == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Error"));
		GameStateTemp->SetNumEnemies(SpawnedEnemies);
	}

	// @todo: Investigate call round row by index and not by name
	FString RoundName = RoundName.FromInt(GetGameState<ARGX_ScoreGameState>()->GetRound());
	RoundName = "Round" + RoundName;
	const FName FRoundName = FName(RoundName);
	const FRGX_RoundDataTable* RoundInfo = DTRounds->FindRow<FRGX_RoundDataTable>(FRoundName, "");

	const TArray<FName> EnemyNames = DTEnemies->GetRowNames();
	const int NumEnemies = EnemyNames.Num();

	// For each enemy type
	for (int i = 0; i < NumEnemies; ++i)
	{
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Purple, EnemyNames[i].ToString());

		// For each enemy to spawn
		for(int j = 0; j < RoundInfo->EnemiesToSpawn[i]; ++j)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Enemy Spawn Call"));
			SpawnEnemy(DTEnemies->FindRow<FRGX_EnemiesDataTable>(EnemyNames[i], "")->EnemyInfo);
			++SpawnedEnemies;
		}
	}
		
	GameStateTemp->SetNumEnemies(SpawnedEnemies);
}

void ARGX_RoundGameMode::SpawnEnemy(UDataAsset* EnemyInfo)
{
	const URGX_EnemyDataAsset* EnemyInfoCasted = Cast<URGX_EnemyDataAsset>(EnemyInfo);
	
	if(EnemyInfoCasted->EnemyBP && EnemySpawners.Num() > 0)
	{
		const int Rand = FMath::RandRange(0, EnemySpawners.Num()-1);
		if(EnemySpawners[Rand])
		{
			if (ARGX_EnemyBase* Enemy = (Cast<ARGX_EnemySpawner>(EnemySpawners[Rand])->Spawn(EnemyInfoCasted->EnemyBP)))
			{
				Enemy->TargetActor = TargetActor;
			}
		}
		
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Spawn"));
	}
	
}

/*
void ARGX_ScoreGameMode::PopulateRoundMap(int Round)
{
	
}*/

void ARGX_RoundGameMode::PopulateSpawnerList()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARGX_EnemySpawner::StaticClass(), EnemySpawners);

	// Debug
	for (int i = 0; i<EnemySpawners.Num(); ++i)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::White, EnemySpawners[i]->GetActorLabel());
		UE_LOG(LogTemp, Warning, TEXT("Spawner Detected"));
	}
}

void ARGX_RoundGameMode::StartPlayEvent_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StartPlay Default Called"));
	
}

void ARGX_RoundGameMode::StartGameSpawn()
{
	if (AActor* Target = UGameplayStatics::GetPlayerCharacter(GetWorld(),0))
	{
		TargetActor = Target;
	}

	SpawnEnemies();
}

void ARGX_RoundGameMode::CleanCorpses()
{
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FGameplayTag::RequestGameplayTag(FName("Status.Dead")).GetTagName(), AllActors);

	for (AActor* Actor : AllActors)
	{
		Actor->Destroy();
	}
}
