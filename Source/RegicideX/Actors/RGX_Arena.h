// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RegicideX/Data/RGX_EnemiesDataTable.h"
#include "RGX_Arena.generated.h"

class ARGX_CombatManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FArenaActivatedSignature, class ARGX_Arena*, ArenaActivated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FArenaDeactivatedSignature, ARGX_Arena*, ArenaDeactivated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FArenaOnEnemyKilledSignature, class ARGX_EnemyBase*, EnemyKilled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaveSpawnedSignature, class URGX_OngoingWave*, SpawnedWave);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaveFinishedSignature, URGX_OngoingWave*, FinishedWave);

// TODO: Move assignation to callback. Unused Function right now
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSpawnEnemyCallbackSignature, ARGX_EnemyBase*, Enemy, URGX_OngoingWave*, Wave);

/* This enum maps to the ArenaWon parameter in FMOD */
UENUM(BlueprintType)
enum class ERGX_ArenaState : uint8
{
	NOT_WON	= 0	UMETA(DisplayName = "NOT_WON"),
	WON			UMETA(DisplayName = "WON")
};

UCLASS(BlueprintType)
class REGICIDEX_API URGX_OngoingWave : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * @brief Data of the outgoing wave
	 */
	UPROPERTY(BlueprintReadOnly)
	URGX_ArenaWaveDataAsset* WaveData;
	int32 EnemiesLeft = 0;
	bool bEnemiesSpawned = false;

	FWaveFinishedSignature OnWaveFinished;

public:
	UFUNCTION()
	void OnEnemyDeath(ARGX_EnemyBase* Enemy);
};

/* This class has a shape which represents the arena where the player will fight. All actors that add logic to said arena
* must be inside this shape (like spawners) to have an effect. The arena is activated by event when the player enters
* the shape, and there should be some guarantee the player does not leave until the arena is finished.
* When there are no more enemies to kill, the arena is deactivated and the player can move on.
*/
UCLASS()
class REGICIDEX_API ARGX_Arena : public AActor
{
	GENERATED_BODY()
	
public:	
	ARGX_Arena();
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ActivateArena();

	UFUNCTION(BlueprintCallable)
	void DeactivateArena();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/* Get all spawners overlapping shape and initialize the EnemySpawners array */
	void InitializeSpawners();

	void SpawnInitialWaves();
	void SpawnWave(URGX_OngoingWave* Wave);
	void SpawnWaveEnemyTypeGroup(const FName& EnemyWaveName, int32 NumEnemies, URGX_OngoingWave* Wave);
	void SpawnWaveEnemy(TSubclassOf<class ARGX_EnemyBase> EnemyClass, int32 SpawnerIdx, URGX_OngoingWave* Wave);
	void SpawnConstantPeasant();
	
	UFUNCTION()
	void HandleSpawnWave(URGX_OngoingWave* Wave);

	void SpawnWaveEnemyRandomMode(TSubclassOf<class ARGX_EnemyBase> EnemyClass, URGX_OngoingWave* Wave);
	void SpawnWaveEnemyRoundRobinMode(TSubclassOf<class ARGX_EnemyBase> EnemyClass, URGX_OngoingWave* Wave);

	UFUNCTION()
	void OnHandleFinishWave(URGX_OngoingWave* FinishedWave);
	UFUNCTION()
	void HandleFinishWave(URGX_OngoingWave* FinishedWave);
	UFUNCTION()
	void HandleFinishArena();

	UFUNCTION()
	void OnEnemyDeath(ARGX_EnemyBase* Enemy);

	UFUNCTION()
	void OnConstantPeasantDeath(ARGX_EnemyBase* Enemy);

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ArenaName;

	UPROPERTY(BlueprintAssignable)
	FArenaActivatedSignature OnArenaActivated;
	UPROPERTY(BlueprintAssignable)
	FArenaDeactivatedSignature OnArenaDeactivated;
	UPROPERTY(BlueprintAssignable)
	FArenaOnEnemyKilledSignature OnArenaEnemyKilled;
	UPROPERTY(BlueprintAssignable)
	FWaveSpawnedSignature OnWaveSpawned;
	UPROPERTY(BlueprintAssignable)
	FWaveFinishedSignature OnWaveFinished;

	// TODO: Move assignation to callback. Unused Function right now
	UPROPERTY()
	FSpawnEnemyCallbackSignature SpawnWaveEnemyCB;
	
	UFUNCTION(BlueprintCallable)
	void SpawnWaveEnemyCallback(ARGX_EnemyBase* Enemy, URGX_OngoingWave* Wave);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsFinalArena = false;
	bool bActivated = false;
	bool bFinished = false;
	bool bIsInitialized = false;
	bool bInitialWavesSpawned = false;

	int LastSpawnerIdx;

	// Constant Spawn of Zombies
	UPROPERTY(EditAnywhere, Category = "Constant Spawn")
	TSubclassOf<ARGX_EnemyBase> PeasantClass;

	// Max number of constant peasants that can be in the arena at any time
	UPROPERTY(EditAnywhere, Category = "Constant Spawn")
	int32 MaxNumConstantPeasants;

	// Initial number of constant peasants that will spawn when the arena is activated
	UPROPERTY(EditAnywhere, Category = "Constant Spawn")
	int32 InitialConstantPeasants;

	UPROPERTY(EditAnywhere, Category = "Constant Spawn")
	float ConstantPeasantSpawnRate;

	float ConstantPeasantLastSpawnTime;
	int32 CurrentNumberConstantPeasant;

	int32 EnemiesLeft;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* ArenaArea;

	UPROPERTY(EditDefaultsOnly, Category = Spawn)
	TSubclassOf<class ARGX_EnemySpawner> EnemySpawnerClass;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<ARGX_EnemySpawner*>  EnemySpawners;

	/**
	 * @brief Reference to player character
	 */
	UPROPERTY()
	class ARGX_PlayerCharacter* PlayerCharacter;

	// TODO: Pass this info to game mode or some static class
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	const UDataTable* DT_EnemyRefs = nullptr;

	UPROPERTY(EditAnywhere, Category = Wave)
	TArray<URGX_ArenaWaveDataAsset*> InitialWavesDataAssets;

	// TODO: Make it a linked list or make use again of the objects instead of destroying them or only allow one child wave
	UPROPERTY()
	TArray<URGX_OngoingWave*> CurrentWaves;

	float TimeBetweenWaves = 2.0f;

	UPROPERTY()
	TWeakObjectPtr<ARGX_CombatManager> CombatManager;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetArenaActivated() const { return bActivated; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetArenaFinished() const { return bFinished; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsFinalArena() const { return bIsFinalArena; }
};
