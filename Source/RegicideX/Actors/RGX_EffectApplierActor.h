// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GenericTeamAgentInterface.h"
#include "RegicideX/GAS/RGX_PayloadObjects.h"
#include "RGX_EffectApplierActor.generated.h"

UCLASS()
class REGICIDEX_API ARGX_EffectApplierActor : public AActor, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:	
	ARGX_EffectApplierActor();

public:
	/** Assigns Team Agent to given TeamID */
	void SetGenericTeamId(const FGenericTeamId& TeamID) override;

	/** Retrieve team identifier in form of FGenericTeamId */
	FGenericTeamId GetGenericTeamId() const override;

public:
	UFUNCTION(BlueprintCallable)
	bool IsPlayerOverlapping(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	bool OnPlayerOverlaps(AActor* Player);

protected:
	// An array of gameplay effects with assigned payloads to the target.
	UPROPERTY(EditDefaultsOnly)
	TArray<FRGX_EffectContextContainer> EffectToApplyToTargetWithPayload;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGenericTeamId ActorTeam;

};
