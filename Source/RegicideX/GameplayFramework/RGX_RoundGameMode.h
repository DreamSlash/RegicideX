// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"

#include "RGX_RoundGameMode.generated.h"


/**
 * 
 */
UCLASS()
class REGICIDEX_API ARGX_RoundGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
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

	virtual void BeginPlay() override;
	/*
	UFUNCTION(BlueprintCallable)
	void PopulateRoundMap(int Round);
	*/
};