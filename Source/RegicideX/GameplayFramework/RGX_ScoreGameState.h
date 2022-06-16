// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RGX_ScoreGameState.generated.h"

/**
 * 
 */
UCLASS()
class REGICIDEX_API ARGX_ScoreGameState : public AGameStateBase
{
	GENERATED_BODY()


protected:
	
	UPROPERTY()
	int Score;

	UPROPERTY()
	int Round;

	UPROPERTY()
	int NumEnemies;
	
public:

	
	ARGX_ScoreGameState();
	virtual ~ARGX_ScoreGameState() = default;

	/** Sets Score, Enemies and Rounds to default values for game start **/
	UFUNCTION(BlueprintCallable)
		void SetStateDefaults();

	/** Adds score depending on type, decreases enemy amount. Returns if there are no enemies remaining **/
	UFUNCTION(BlueprintCallable)
		bool OnEnemyDeath(int Type);

	UFUNCTION(BlueprintCallable)
		void SetRound(int NewRound) { Round = NewRound; }
	/** Manages the game state changes for round change. **/

	UFUNCTION(BlueprintCallable)
		int StartNextRound();

	UFUNCTION(BlueprintPure)
		int GetScore() const;

	UFUNCTION(BlueprintPure)
		int GetRound() const;
	
	UFUNCTION(BlueprintPure)
		int GetNumEnemies() const;

	UFUNCTION(BlueprintCallable)
		void SetScore(const int NewScore);
	
	UFUNCTION(BlueprintCallable)
		void SetNumEnemies(const int NewNumEnemies);

	UFUNCTION(BlueprintCallable)
		void IncreaseScore();

};



