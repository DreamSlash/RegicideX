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

	UPROPERTY()
	APawn* TargetActor;

	UPROPERTY()
	TArray<ARGX_Peasant*> PeasantArray;

	UPROPERTY()
	TArray<ARGX_Peasant*> PeasantsInCombat;

	UPROPERTY()
	TArray<FVector> PositionsAroundTarget;

	UPROPERTY()
	FVector LastPlayerLocation;

	UPROPERTY()
	TMap<ARGX_Peasant*, FVector> PeasantToPosition;
	
public:	
	// Sets default values for this actor's properties
	ARGX_GroupManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditDefaultsOnly)
	unsigned int MaxAttackers;
	UPROPERTY(EditDefaultsOnly)
	unsigned int CurrentAttackers;
	UPROPERTY(EditDefaultsOnly)
	unsigned int MaxNumberOfPeasants;
	UPROPERTY(EditDefaultsOnly)
	unsigned int CurrentNumberOfPeasants;

	UPROPERTY(EditDefaultsOnly)
	UEnvQuery* PositionQuery;

	UPROPERTY(EditDefaultsOnly)
	FEnvQueryRequest PositionQueryRequest;

	UPROPERTY(EditDefaultsOnly)
	bool bCanSpawn = false;

	void PositionEQSFinished(TSharedPtr<FEnvQueryResult> Result);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddPeasant(ARGX_Peasant* newPeasant);

	void RemovePeasant(ARGX_Peasant* PeasantToRemove);

	void RecalcPositions();

	void OnPeasantAdded();

	void RecalcPeasants();
};
