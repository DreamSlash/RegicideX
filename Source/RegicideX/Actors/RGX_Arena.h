// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RGX_Arena.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FArenaActivateSignature, class ARGX_Arena*, ArenaActivated);

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

public:
	FArenaActivateSignature OnArenaActivated;

private:
	bool bActivated = false;
	bool bFinished = false;
	bool bIsInitialized = false;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* ArenaArea;

	UPROPERTY(EditDefaultsOnly, Category = Spawner)
	TSubclassOf<class ARGX_EnemySpawner> EnemySpawnerClass;

	TArray<ARGX_EnemySpawner*>  EnemySpawners;
};
