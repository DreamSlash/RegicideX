// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_RoundGameMode.h"

#include "RGX_ScoreGameState.h"
#include "Engine/AssetManager.h"
#include "RegicideX/Data/RGX_EnemyDataAsset.h"
#include "RegicideX/Data/RGX_RoundDataTable.h"

ARGX_RoundGameMode::ARGX_RoundGameMode()
{
	GameStateClass = ARGX_ScoreGameState::StaticClass();
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

	ARGX_ScoreGameState* GameStateTemp = GetGameState<ARGX_ScoreGameState>(); // @todo: Reduce Calls to GetGameState
	GameStateTemp->SetStateDefaults();
	
	Super::StartPlay(); //Must be at the end
}

void ARGX_RoundGameMode::BeginPlay()
{
	GetGameState<ARGX_ScoreGameState>()->SetNumEnemies(SpawnEnemies());
	Super::BeginPlay();
}

void ARGX_RoundGameMode::OnEnemyDeath(const int Type)
{
	if(GetGameState<ARGX_ScoreGameState>()->OnEnemyDeath(Type))
	{
		NextRound();
	}
}

void ARGX_RoundGameMode::NextRound()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, TEXT("Next Round"));
	GetGameState<ARGX_ScoreGameState>()->NextRound();
	GetGameState<ARGX_ScoreGameState>()->SetNumEnemies(SpawnEnemies());
}

int ARGX_RoundGameMode::SpawnEnemies()
{

	if(UAssetManager* Manager = UAssetManager::GetIfValid())
	{

		/*
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
		}*/
		
		int SpawnedEnemies = 0;
	
		FString RoundName = RoundName.FromInt(GetGameState<ARGX_ScoreGameState>()->GetRound());
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
				SpawnEnemy(DTEnemies->FindRow<FRGX_EnemiesDataTable>(EnemyNames[i], "")->EnemyInfo);
				SpawnedEnemies++;
			}
		}
	
		return SpawnedEnemies;
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Error"));
	
	return 0;

}

void ARGX_RoundGameMode::SpawnEnemy(UDataAsset* EnemyInfo)
{
	//@todo: Get List of spawners, decide which spawner to use, call spawn enemy

	URGX_EnemyDataAsset* EnemyInfoCasted = Cast<URGX_EnemyDataAsset>(EnemyInfo);
}

/*
void ARGX_ScoreGameMode::PopulateRoundMap(int Round)
{
	
}*/

void ARGX_RoundGameMode::StartPlayEvent_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StartPlay Default Called"));
	
}
