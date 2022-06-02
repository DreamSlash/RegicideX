// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_RoundGameMode.h"
#include "GameFramework/Actor.h"
#include "RGX_ScoreGameState.h"
#include "Engine/AssetManager.h"
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
	GetWorld()->GetTimerManager().SetTimer(FirstSpawnTimerHandle, this, &ARGX_RoundGameMode::StartGameSpawn, 5.0f, false);
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
	ARGX_ScoreGameState* GameStateTemp = GetGameState<ARGX_ScoreGameState>(); // @todo: Reduce Calls to GetGameState
	GameStateTemp->NextRound();
	GameStateTemp->SetNumEnemies(SpawnEnemies());
}

int ARGX_RoundGameMode::SpawnEnemies()
{

	if(UAssetManager* Manager = UAssetManager::GetIfValid())
	{
		if(!DTRounds || !DTEnemies)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Error"));
			return 0;
		}

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
				
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Enemy Spawn Call"));
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

	for (int i = 0; i<EnemySpawners.Num();i++)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::White, EnemySpawners[i]->GetActorLabel());
		UE_LOG(LogTemp, Warning, TEXT("Spawner Detected"));
	}
}

void ARGX_RoundGameMode::StartPlayEvent_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StartPlay Default Called"));
	
}

void ARGX_RoundGameMode::StartGameSpawn()
{
	if (AActor* Target = UGameplayStatics::GetPlayerCharacter(GetWorld(),0))
	{
		TargetActor = Target;
	}

	GetGameState<ARGX_ScoreGameState>()->SetNumEnemies(SpawnEnemies());
}