// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RGX_PeasantController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

/**
 * 
 */
UCLASS()
class REGICIDEX_API ARGX_PeasantController : public AAIController
{
	GENERATED_BODY()
public:

	ARGX_PeasantController();

	UPROPERTY(transient)
	UBlackboardComponent* BBComponent;

	UPROPERTY(transient)
	UBehaviorTreeComponent* BTComponent;

	virtual void OnPossess(APawn* pawn) override;
	virtual void Tick(float DeltaTime) override;

	// Blackboard key ids
	uint8 TargetActorID;
	uint8 PositionID;
	uint8 WasHitID;
	uint8 OnAirID;
	uint8 InCombatID;
	uint8 IdlePositionID;

protected:
};
