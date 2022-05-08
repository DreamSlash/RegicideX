// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_ScoreGameMode.h"

#include "RGX_ScoreGameState.h"
#include "Engine/AssetManager.h"

ARGX_ScoreGameMode::ARGX_ScoreGameMode()
{
	GameStateClass = ARGX_ScoreGameState::StaticClass();
}

int ARGX_ScoreGameMode::GetScore() const
{
	return GetGameState<ARGX_ScoreGameState>()->Score;
}

void ARGX_ScoreGameMode::SetScore(const int NewScore)
{
	GetGameState<ARGX_ScoreGameState>()->Score = NewScore;

}

int ARGX_ScoreGameMode::GetRound() const
{
	return GetGameState<ARGX_ScoreGameState>()->Round;
}

void ARGX_ScoreGameMode::StartPlay()
{
	StartPlayEvent();

	GetGameState<ARGX_ScoreGameState>()->Score = 0;
	GetGameState<ARGX_ScoreGameState>()->Round = 1;
	GetGameState<ARGX_ScoreGameState>()->NumEnemies = 0;
	
	Super::StartPlay(); //Must be at the end
}

void ARGX_ScoreGameMode::BeginPlay()
{
	GetGameState<ARGX_ScoreGameState>()->NumEnemies += SpawnEnemies(GetGameState<ARGX_ScoreGameState>()->Round);
	Super::BeginPlay();
}

void ARGX_ScoreGameMode::EnemyDeath(const int Type)
{
	if(GetGameState<ARGX_ScoreGameState>()->EnemyDeath(Type))
	{
		NextRound();
	}
	
}

void ARGX_ScoreGameMode::NextRound()
{

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, TEXT("Next Round"));
			
	GetGameState<ARGX_ScoreGameState>()->NextRound();
	
	GetGameState<ARGX_ScoreGameState>()->NumEnemies += SpawnEnemies(GetGameState<ARGX_ScoreGameState>()->Round);
	
}

int ARGX_ScoreGameMode::SpawnEnemies(const int Round)
{

	if(UAssetManager* Manager = UAssetManager::GetIfValid())
	{

		FPrimaryAssetId AssetId = GetPrimaryAssetId();
		
		const FName AssetType = FName("EnemyInfo");
		const FPrimaryAssetType* EnemyType = new FPrimaryAssetType(AssetType);

		TArray<FPrimaryAssetId> EnemyList;
		Manager->GetPrimaryAssetIdList(*EnemyType,EnemyList);

		for (const FPrimaryAssetId& EnemyId : EnemyList)
		{
			FAssetData AssetDataToParse;
			Manager->GetPrimaryAssetData(EnemyId, AssetDataToParse);

			// WIP
		}


		
		int SpawnedEnemies = 0;
	
		FString RoundName = RoundName.FromInt(Round);
		RoundName = "Round" + RoundName;
		const FName FRoundName = FName(RoundName);
		FRGX_RoundDataTable* RoundInfo = DTRounds->FindRow<FRGX_RoundDataTable>(FRoundName, "");

		const TArray<FName> EnemyNames = DTEnemies->GetRowNames();
		const int NumEnemies = EnemyNames.Num();

		for (int i = 0; i < NumEnemies; i++)
		{
			GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Purple, EnemyNames[i].ToString());
				
			for(int j = 0; j < RoundInfo->EnemiesToSpawn[i]; j++)
			{
				
				GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, TEXT("Enemy Spawn"));
				//SpawnEnemy(DTEnemies->FindRow<FRGX_EnemiesDataTable>(EnemyNames[i], "")->EnemyInfo);
				SpawnedEnemies++;
			}
		}
	
		return SpawnedEnemies;
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Error"));
	
	return 0;

}

void ARGX_ScoreGameMode::SpawnEnemy(ARGX_EnemyBase* EnemyToSpawn)
{
	//@todo: Get List of spawners, decide which spawner to use, call spawn enemy
}

/*
void ARGX_ScoreGameMode::PopulateRoundMap(int Round)
{
	
}*/

void ARGX_ScoreGameMode::StartPlayEvent_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StartPlay Default Called"));
	
}
