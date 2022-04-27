// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RGX_GroupManager.generated.h"

class ARGX_Peasant;
class APawn;

UCLASS()
class REGICIDEX_API ARGX_GroupManager : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	APawn* TargetActor;

	UPROPERTY()
	TArray<ARGX_Peasant*> PeasantArray;
	
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

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddPeasant(ARGX_Peasant* newPeasant);

	void RemovePeasant(ARGX_Peasant* PeasantToRemove);
};
