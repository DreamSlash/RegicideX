// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RegicideX/Data/RGX_EnemiesDataTable.h"
#include "RegicideX/Data/RGX_RoundDataTable.h"
#include "RGX_ScoreGameMode.generated.h"

/**
 * 
 */
UCLASS()
class REGICIDEX_API ARGX_ScoreGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, int> WaveSpawnsMap;
	*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* DTEnemies;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* DTRounds;
	
	ARGX_ScoreGameMode();
	virtual ~ARGX_ScoreGameMode() = default;

	/** Return score **/
	UFUNCTION(BlueprintPure)
	virtual int GetScore() const;

	/** Set score: use for test purposes only **/
	UFUNCTION(BlueprintCallable)
	void SetScore(int NewScore);

	/** Return Round **/
	UFUNCTION(BlueprintCallable)
	int GetRound() const;

	virtual void StartPlay() override;

	UFUNCTION(BlueprintNativeEvent, DisplayName="StartPlay")
	void StartPlayEvent();

	/** To call when an enemy dies **/
	UFUNCTION(BlueprintCallable)
	void EnemyDeath(int Type);

	/** Increments the round count and handles all the round logic **/
	UFUNCTION(BlueprintCallable)
	void NextRound();

	/** Spawns enemies in the defined spawn points depending on the round. Returns the amount of enemies spawned. **/
	UFUNCTION(BlueprintCallable)
	int SpawnEnemies(int Round);

	/** Spawns enemy of the defined class **/
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy(ARGX_EnemyBase* EnemyToSpawn);

	virtual void BeginPlay() override;
	/*
	UFUNCTION(BlueprintCallable)
	void PopulateRoundMap(int Round);
	*/
};