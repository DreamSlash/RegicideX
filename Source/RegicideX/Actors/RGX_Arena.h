// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RegicideX/Data/RGX_EnemiesDataTable.h"
#include "RGX_Arena.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FArenaActivatedSignature, class ARGX_Arena*, ArenaActivated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FArenaDeactivatedSignature, ARGX_Arena*, ArenaDeactivated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FArenaOnEnemyKilledSignature, class ARGX_EnemyBase*, EnemyKilled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaveFinishedSignature, class URGX_OutgoingWave*, FinishedWave);

UCLASS()
class REGICIDEX_API URGX_OutgoingWave : public UObject
{
	GENERATED_BODY()

public:
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

public:
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
	void SpawnWave(URGX_OutgoingWave* Wave);
	void SpawnWaveEnemyTypeGroup(const FName& EnemyWaveName, int32 NumEnemies, URGX_OutgoingWave* Wave);
	void SpawnWaveEnemy(TSubclassOf<class ARGX_EnemyBase> EnemyClass, int32 SpawnerIdx, URGX_OutgoingWave* Wave);
	void SpawnConstantPeasant();

	UFUNCTION()
	void OnHandleFinishWave(URGX_OutgoingWave* FinishedWave);
	UFUNCTION()
	void HandleFinishWave(URGX_OutgoingWave* FinishedWave);

	void HandleFinishArena();

	UFUNCTION()
	void OnComponentBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UFUNCTION()
	void OnEnemyDeath(ARGX_EnemyBase* Enemy);

	UFUNCTION()
	void OnConstantPeasantDeath(ARGX_EnemyBase* Enemy);

public:
	FArenaActivatedSignature OnArenaActivated;
	FArenaDeactivatedSignature OnArenaDeactivated;
	FArenaOnEnemyKilledSignature OnArenaEnemyKilled;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsFinalArena = false;
	bool bActivated = false;
	bool bFinished = false;
	bool bIsInitialized = false;
	bool bInitialWavesSpawned = false;

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

	TArray<ARGX_EnemySpawner*>  EnemySpawners;

	class ARGX_PlayerCharacter* PlayerCharacter;

	// TODO: Pass this info to game mode or some static class
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	const UDataTable* DT_EnemyRefs = nullptr;

	UPROPERTY(EditAnywhere, Category = Wave)
	TArray<URGX_ArenaWaveDataAsset*> InitialWavesDataAssets;

	// TODO: Make it a linked list or make use again of the objects instead of destroying them or only allow one child wave
	UPROPERTY()
	TArray<URGX_OutgoingWave*> CurrentWaves;

	float TimeBetweenWaves = 2.0f;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetArenaActivated() const { return bActivated; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetArenaFinished() const { return bFinished; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsFinalArena() const { return bIsFinalArena; }
};
