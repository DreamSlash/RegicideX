// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/DataTable.h"

#include "RGX_LinearArenasGameMode.generated.h"

class ULevelSequence;
class ULevelSequencePlayer;

UCLASS()
class REGICIDEX_API ARGX_LinearArenasGameMode : public AGameModeBase
{
	GENERATED_BODY()

		DECLARE_DELEGATE(FOnEndingCutsceneFinishedDelegate);

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
	void BP_OnWaveSpawned(class URGX_OngoingWave* SpawnedWave);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnWaveFinished(URGX_OngoingWave* FinishedWave);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnPlayerWins();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_OnPlayerLoses();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_OnTriggerCredits();

private:
	UFUNCTION()
	void OnArenaActivated(class ARGX_Arena* ActivatedArena);

	UFUNCTION()
	void OnArenaDeactivated(ARGX_Arena* DeactivatedArena);

	UFUNCTION()
	void OnArenaEnemyKilled(class ARGX_EnemyBase* EnemyKilled);

	UFUNCTION()
	void OnWaveSpawned(URGX_OngoingWave* SpawnedWave);

	UFUNCTION()
	void OnWaveFinished(URGX_OngoingWave* FinishedWave);

public:
	UFUNCTION()
	void TriggerCredits();

	void FinalArenaFinished();

	void SkipCutscene();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	const UDataTable* DT_EnemyTypesInfo = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> CreditsWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> PlayerWinsWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	int32 Score;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ULevelSequence* EndingSequence = nullptr;

private:
    ULevelSequencePlayer* currentLevelSequencePlayer = nullptr;
	UUserWidget* CreditsWidget = nullptr;
    ARGX_Arena* CurrentArena;
    TArray<ARGX_Arena*> Arenas;
    bool bWinCondition = false;
};
