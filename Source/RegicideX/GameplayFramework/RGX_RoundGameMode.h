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
	
public:
	
	

	// @todo: Map DataTables On BeginPlay/StartPlay
	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, int> WaveSpawnsMap;
	*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* DTEnemies;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* DTRounds;

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetEnemySpawners() const;

	UFUNCTION(BlueprintCallable)
	void SetEnemySpawners(const TArray<AActor*>& EnemySpawnersList);
	
	ARGX_RoundGameMode();
	virtual ~ARGX_RoundGameMode() = default;

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
	void NextRound();

	/** Spawns enemies in the defined spawn points depending on the round. Returns the amount of enemies spawned. **/
	UFUNCTION(BlueprintCallable)
	int SpawnEnemies();

	/** Spawns enemy of the defined class **/
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy(UDataAsset* EnemyInfo);

	UFUNCTION(BlueprintCallable)
	void PopulateSpawnerList();

	UPROPERTY()
		AActor* TargetActor = nullptr;

private:
	void StartGameSpawn();

private:
	FTimerHandle FirstSpawnTimerHandle;
	
	/*
	UFUNCTION(BlueprintCallable)
	void PopulateRoundMap(int Round);
	*/
};


