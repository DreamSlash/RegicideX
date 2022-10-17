// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RegicideX/AI/Controllers/RGX_EnemyBaseController.h"
#include "RGX_MageAngelController.generated.h"

UCLASS()
class REGICIDEX_API ARGX_MageAngelController : public ARGX_EnemyBaseController
{
	GENERATED_BODY()

protected:
	void BeginPlay() override;
	void OnPossess(APawn* InPawn) override;

};
