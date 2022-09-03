// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RegicideX/Data/RGX_EnemiesDataTable.h"
#include "RGX_Arena.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FArenaActivatedSignature, class ARGX_Arena*, ArenaActivated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FArenaDeactivatedSignature, class ARGX_Arena*, ArenaDeactivated);

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

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/* Get all spawners overlapping shape and initialize the EnemySpawners array */
	void InitializeSpawners();

	void SpawnWave();
	void SpawnEnemyTypeGroup(const FName& EnemyWaveName, int32 NumEnemies);
	void SpawnEnemy(TSubclassOf<class ARGX_EnemyBase> EnemyClass, int32 SpawnerNum);
	void HandleFinishWave();
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
	void OnEnemyDeath(int32 Score);

public:
	FArenaActivatedSignature OnArenaActivated;
	FArenaDeactivatedSignature OnArenaDeactivated;

private:
	bool bActivated = false;
	bool bFinished = false;
	bool bIsInitialized = false;
	bool bEnemiesSpawned = false;

	//UPROPERTY(EditAnywhere, Category = Spawn)
	//int32 NumEnemiesToSpawn;

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
	URGX_ArenaWaveDataAsset* WaveDataAsset = nullptr;
};
