// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RGX_EnemyBase.h"
#include "RGX_MageAngel.generated.h"

class USceneComponent;

/**
 * 
 */
UCLASS()
class REGICIDEX_API ARGX_MageAngel : public ARGX_EnemyBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* RingWaveSource = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float HeightPos = 500.0f;

public:
	ARGX_MageAngel();

	void BeginPlay() override;
	void Tick(float DeltaTime) override;

public:
	void SetToBeDestroyed() { bToBeDestroyed = true; }

private:
	bool bToBeDestroyed = false;

};
