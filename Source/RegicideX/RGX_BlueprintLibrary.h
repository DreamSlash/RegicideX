// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RegicideX/GAS/RGX_PayloadObjects.h"
#include "RegicideX/Actors/RGX_CharacterBase.h"
#include "RGX_BlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class REGICIDEX_API URGX_BlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = Ability)
	static void ApplyGameplayEffectContextContainerToActor(const ARGX_CharacterBase* CharacterBase, TSubclassOf<UGameplayEffect> EffectToApply, URGX_RGXEventDataAsset* Payload);

	UFUNCTION(BlueprintCallable, Category = Character)
	static void LaunchCharacterToLocation(ARGX_CharacterBase* CharacterBase, const FVector& TargetLocation);
};