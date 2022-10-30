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
	static void LaunchCharacterToLocation(ARGX_CharacterBase* CharacterBase, const FVector& TargetLocation, float GravityScaleMultiplier, float LaunchAngle);

	UFUNCTION(BlueprintCallable, Category = Character)
	static bool ConeCheck(ARGX_CharacterBase* OriginCharacter, AActor* DestinationActor, float DotThreshold, bool bIgnoreZAxis);

	UFUNCTION(BlueprintPure)
	static FName GetPackageFromLevelAsset(const TSoftObjectPtr<UWorld>& World);

	UFUNCTION(BlueprintCallable)
	static bool TranslateCharacterMeshToPoint(ACharacter* Actor, FVector GoalPoint, float Alpha, float Tolerance);
};