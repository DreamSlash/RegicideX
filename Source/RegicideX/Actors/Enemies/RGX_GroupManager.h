// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/Actor.h"
#include "RGX_GroupManager.generated.h"

class ARGX_Peasant;
class APawn;
class UEnvQuery;

UCLASS()
class REGICIDEX_API ARGX_GroupManager : public AActor
{
	GENERATED_BODY()

	// Player character
	UPROPERTY()
	APawn* TargetActor;

	// Array containing all peasants in the manager
	UPROPERTY()
	TArray<ARGX_Peasant*> PeasantArray;

	// In combat
	UPROPERTY()
	TArray<ARGX_Peasant*> PeasantsInCombat;

	// Map of peasant for each position
	UPROPERTY()
	TMap<ARGX_Peasant*, FVector> PeasantToPosition;

	UPROPERTY()
	TArray<FVector> PositionsAroundTarget;

	UPROPERTY()
	FVector LastPlayerLocation;


	
public:	
	// Sets default values for this actor's properties
	ARGX_GroupManager();

	// Peasant from the attacking group who's gonna perform the combat action.
	UPROPERTY()
	ARGX_Peasant* AttackingPeasant = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditDefaultsOnly)
	unsigned int MaxAttackers;
	UPROPERTY()
	unsigned int CurrentAttackers;
	UPROPERTY(EditDefaultsOnly)
	unsigned int MaxNumberOfPeasants;
	UPROPERTY()
	unsigned int CurrentNumberOfPeasants;
	UPROPERTY(EditDefaultsOnly)
	float OffsetPlayer;

	UPROPERTY(EditDefaultsOnly)
	UEnvQuery* PositionQuery;

	UPROPERTY(EditDefaultsOnly)
	FEnvQueryRequest PositionQueryRequest;

	UPROPERTY()
	bool bCanSpawn = false;

	void PositionEQSFinished(TSharedPtr<FEnvQueryResult> Result);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddPeasant(ARGX_Peasant* newPeasant);

	void RemovePeasant(ARGX_Peasant* PeasantToRemove);

	void RecalcPositions();

	void Invalidate();

	void OnPeasantAdded();

	void RecalcPeasants();
};
