// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RegicideX/Actors/RGX_PoolActor.h"
#include "RGX_PoolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REGICIDEX_API URGX_PoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URGX_PoolComponent();
	ARGX_PoolActor* GetPooledActor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = "PoolActor")
		TSubclassOf<ARGX_PoolActor> PooledActorSubclass;
	
	UPROPERTY(EditAnywhere, Category = "ObjectPooler")
		int PoolSize = 100;

	TArray<ARGX_PoolActor*> Pool;

};
