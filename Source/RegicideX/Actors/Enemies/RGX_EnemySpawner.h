// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RGX_EnemyBase.h"
#include "GameFramework/Actor.h"
//#include "RegicideX/Components/RGX_PoolComponent.h"
#include "RGX_EnemySpawner.generated.h"

class ARGX_GroupManager;

UCLASS()
class REGICIDEX_API ARGX_EnemySpawner : public AActor
{
	GENERATED_BODY()

private:
	FTimerDelegate TimerDel;
	FTimerHandle TimerHandle;
	TArray<TSubclassOf<ARGX_EnemyBase>> EnemyBufferToSpawn;
	
public:
	// Sets default values for this actor's properties
	ARGX_EnemySpawner();

	FTimerHandle SpawnTimerHandle;

	UPROPERTY(EditAnywhere)
	FBox SpawnBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnRadius = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool SpawnerActive = false;
	
	/** Spawns enemy based on the blueprint EnemyBP **/
	UFUNCTION(BlueprintCallable)
	ARGX_EnemyBase* Spawn(TSubclassOf<ARGX_EnemyBase> EnemyBP);
	
	UFUNCTION(BlueprintCallable)
	ARGX_EnemyBase* HandleSpawn(TSubclassOf<ARGX_EnemyBase> EnemyBP);
	
	UFUNCTION(BlueprintImplementableEvent)
	void ActivateSpawner(TSubclassOf<ARGX_EnemyBase> EnemyBP);
	
	/** Function called to spawn BP_Peasant **/
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnPeasant(FTransform Transform);

protected:
	/** Called when the game starts or when spawned **/
	virtual void BeginPlay() override;

};
