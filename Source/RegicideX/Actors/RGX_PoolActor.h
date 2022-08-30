// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RegicideX/Actors/RGX_CharacterBase.h"
#include "RGX_PoolActor.generated.h"

UCLASS()
class REGICIDEX_API ARGX_PoolActor : public ARGX_CharacterBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARGX_PoolActor();

	virtual void SetLifeSpan(float InLifespan) override;
	bool IsActive();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Activate();

	UFUNCTION(BlueprintCallable)
	virtual void Deactivate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	bool bActive = false;

	void SetActive(bool bActive);

	FTimerHandle LifeSpanTimerHandle;
	
	float LifeSpan = 0.0f; // If lifespan is 0.0f, it should not be deactivate by timer.

};
