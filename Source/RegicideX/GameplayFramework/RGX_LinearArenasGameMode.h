// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RGX_LinearArenasGameMode.generated.h"

/**
 * 
 */
UCLASS()
class REGICIDEX_API ARGX_LinearArenasGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void StartPlay() override;

protected:
	virtual void BeginPlay() override;

private:

};
