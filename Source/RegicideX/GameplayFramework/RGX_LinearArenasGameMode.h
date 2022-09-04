// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/DataTable.h"
#include "RGX_LinearArenasGameMode.generated.h"

/**
 * 
 */
UCLASS()
class REGICIDEX_API ARGX_LinearArenasGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void StartPlay() override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnArenaActivated(ARGX_Arena* ActivatedArena);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnArenaDeactivated(ARGX_Arena* DeactivatedArena);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnArenaEnemyKilled();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnPlayerWins();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnPlayerLoses();

private:
	UFUNCTION()
	void OnArenaActivated(class ARGX_Arena* ActivatedArena);

	UFUNCTION()
	void OnArenaDeactivated(ARGX_Arena* DeactivatedArena);

	UFUNCTION()
	void OnArenaEnemyKilled(class ARGX_EnemyBase* EnemyKilled);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	const UDataTable* DT_EnemyTypesInfo = nullptr;

private:
	bool bWinCondition = false;

	ARGX_Arena* CurrentArena;

	TArray<ARGX_Arena*> Arenas;

public:
	UPROPERTY(BlueprintReadWrite)
	int32 Score;
};
