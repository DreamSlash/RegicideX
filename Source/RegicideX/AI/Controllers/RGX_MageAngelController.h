// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RGX_MageAngelController.generated.h"

/**
 * 
 */

UCLASS()
class REGICIDEX_API ARGX_MageAngelController : public AAIController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeConsecutiveHits = 1.0f;

	UFUNCTION(BlueprintCallable)
	void DamageTaken();

protected:
	void BeginPlay() override;

private:
	FTimerHandle TimerHandle;

};
