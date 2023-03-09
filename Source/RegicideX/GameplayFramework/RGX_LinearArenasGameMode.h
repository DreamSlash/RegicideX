// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/DataTable.h"
#include "FMODEvent.h"

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

	UFUNCTION(BlueprintCallable)
	void PlayArenaMusic(UFMODEvent* InEvent);

	UFUNCTION(BlueprintCallable)
	void TriggerArenaWonMusic();

	UFUNCTION(BlueprintCallable)
	void UpdateArenaMusicIntensity(float InValue);

	UFUNCTION(BlueprintCallable)
	void StopArenaMusic();

public:
	UFUNCTION()
	void TriggerCredits();

	void FinalArenaFinished();
	void SkipCutscene();
	void HandlePlayerLoses();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	const UDataTable* DT_EnemyTypesInfo = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ULevelSequence* EndingSequence = nullptr;

	UPROPERTY(BlueprintReadWrite)
	int32 Score = 0;

	UPROPERTY(EditDefaultsOnly, Category = Widget)
	TSubclassOf<UUserWidget> CreditsWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = Widget)
	TSubclassOf<UUserWidget> PlayerWinsWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = Widget)
	TSubclassOf<UUserWidget> PlayerLosesWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
	UFMODEvent* CreditsEvent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
	UFMODEvent* PlayerDiesEvent = nullptr;

private:
    bool bWinCondition = false;

    ULevelSequencePlayer*	CurrentLevelSequencePlayer	= nullptr;
	UUserWidget*			CreditsWidget				= nullptr;
    ARGX_Arena*				CurrentArena;
    TArray<ARGX_Arena*>		Arenas;
};
