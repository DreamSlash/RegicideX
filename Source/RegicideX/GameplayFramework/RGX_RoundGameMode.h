// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RegicideX/Actors/Enemies/RGX_EnemySpawner.h"
#include "RegicideX/Actors/RGX_PoolSpawner.h"
#include "RegicideX/Data/RGX_RoundDataTable.h"

#include "RGX_RoundGameMode.generated.h"

UCLASS()
class REGICIDEX_API ARGX_RoundGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	UPROPERTY()
		int KillCount = 0;

	UPROPERTY()
		int CurrentWave = 1;

	UPROPERTY()
		int SpawnedEnemies = 0;

	UPROPERTY()
		TArray<AActor*> EnemySpawners;

	UPROPERTY()
		TArray<FName> EnemyWaveNames;

	UPROPERTY()
		FRGX_RoundDataTable RoundInfo;

	UPROPERTY()
	AActor* TargetActor = nullptr;
	
public:
	
	// @todo: Replace with Asset Manager
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	const UDataTable* DTEnemies = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	const UDataTable* DTRounds = nullptr;
	
	ARGX_RoundGameMode();
	virtual ~ARGX_RoundGameMode() = default;

	/** Get list of enemy spawners **/
	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetEnemySpawners() const;

	/** Set Enemy Spawner list. For test purposes only **/
	UFUNCTION(BlueprintCallable)
	void SetEnemySpawners(const TArray<AActor*>& EnemySpawnersList);

	/** Return score **/
	UFUNCTION(BlueprintPure)
	int GetScore() const;

	/** Set score: use for test purposes only **/
	UFUNCTION(BlueprintCallable)
	void SetScore(int NewScore);

	/** Return Round **/
	UFUNCTION(BlueprintPure)
	int GetRound() const;

	void StartPlay() override;

	void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, DisplayName="StartPlay")
	void StartPlayEvent();

	UFUNCTION(BlueprintNativeEvent, DisplayName = "StartWaveEvent")
	void StartWaveEvent();

	UFUNCTION(BlueprintNativeEvent, DisplayName = "EndWaveEvent")
	void EndWaveEvent();

	UFUNCTION(BlueprintNativeEvent, DisplayName = "EndGameEvent")
	void EndGameEvent();

	UFUNCTION(BlueprintNativeEvent, DisplayName = "OnEnemyDeadEvent")
	void EnemyDeadEvent();

	UFUNCTION()
	void StartEnemySpawn();

	UFUNCTION()
	void IncreaseKillCount();

private:

	FTimerHandle SpawnTimerHandle;
	
	void StartNewWave();

	void CheckCurrentWave();

	void SpawnNewWave();

	void SpawnEnemyGroups();

	void SpawnEnemy(UDataAsset* EnemyInfo);

	UFUNCTION()
	void OnEnemyDestroyed(int EnemyScoreValue);

	void OnWaveFinished();
};


