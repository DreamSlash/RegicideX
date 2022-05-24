// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RegicideX/Actors/Enemies/RGX_EnemySpawner.h"

#include "RGX_RoundGameMode.generated.h"


/**
 * 
 */
UCLASS()
class REGICIDEX_API ARGX_RoundGameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<AActor*> EnemySpawners;

	UPROPERTY()
	AActor* TargetActor = nullptr;
	
public:
	
	// @todo: Map DataTables On BeginPlay/StartPlay
	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, int> WaveSpawnsMap;
	*/

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

	/** To call when an enemy dies **/
	UFUNCTION(BlueprintCallable)
	void OnEnemyDeath(int Type);

	/** Increments the round count and handles all the round logic **/
	UFUNCTION(BlueprintCallable)
	void StartNextRound();

	/** Spawns enemies in the defined spawn points depending on the round. Returns the amount of enemies spawned. **/
	UFUNCTION(BlueprintCallable)
	int SpawnEnemies();

	/** Spawns enemy of the defined class **/
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy(UDataAsset* EnemyInfo);

	/** Populates the list of enemy spawners by getting them from the current map.
	 * Called at Begin Play **/
	UFUNCTION(BlueprintCallable)
	void PopulateSpawnerList();



private:
	/** Sets TargetActor to Player 0 Character and calls Spawn Enemies. Called at Begin Play**/
	void StartGameSpawn();
	
	FTimerHandle FirstSpawnTimerHandle;
	
	/*
	UFUNCTION(BlueprintCallable)
	void PopulateRoundMap(int Round);
	*/
};


