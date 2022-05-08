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


public:
	ARGX_ScoreGameState();
	virtual ~ARGX_ScoreGameState() = default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Score;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Round;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int NumEnemies;
	
	UFUNCTION(BlueprintCallable)
		bool EnemyDeath(int Type);

	UFUNCTION(BlueprintCallable)
		int NextRound();

	UFUNCTION(BlueprintCallable)
		void SetNumEnemies(int NewNumEnemies);
};



