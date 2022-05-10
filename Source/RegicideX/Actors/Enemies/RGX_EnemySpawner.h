// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RGX_EnemyBase.h"
#include "GameFramework/Actor.h"
#include "RGX_EnemySpawner.generated.h"

class ARGX_GroupManager;

UCLASS()
class REGICIDEX_API ARGX_EnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ARGX_EnemySpawner();

	FTimerHandle SpawnTimerHandle;

	UPROPERTY(EditAnywhere)
	ARGX_GroupManager* Manager;

	UPROPERTY(EditAnywhere)
	FBox SpawnBox;

	UFUNCTION(BlueprintCallable)
	void Spawn(TSubclassOf<ARGX_EnemyBase> EnemyBP);

	/** Function called to spawn BP_Peasant **/
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnPeasant(FTransform Transform);

protected:
	/** Called when the game starts or when spawned **/
	virtual void BeginPlay() override;
};
