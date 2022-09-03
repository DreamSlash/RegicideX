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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
	void OnArenaActivated(class ARGX_Arena* ActivatedArena);

private:
	ARGX_Arena* CurrentArena;

	TArray<ARGX_Arena*> Arenas;
};
