// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/MCV_GameplayAbility.h"
#include "RGX_GameplayAbility.generated.h"

UCLASS()
class REGICIDEX_API URGX_GameplayAbility : public UMCV_GameplayAbility
{
	GENERATED_BODY()
	
};

UENUM(BlueprintType)
enum class ERGX_AbilityInputID : uint8
{
	None				UMETA(DisplayName = "None"),
	Confirm				UMETA(DisplayName = "Confirm"),
	Cancel				UMETA(DisplayName = "Cancel"),
	LightAttack			UMETA(DisplayName = "LightAttack"),
	HeavyAttack			UMETA(DisplayName = "HeavyAttack"),
	Evasion				UMETA(DisplayName = "Evasion"),
	PowerSkill			UMETA(DisplayName = "PowerSkill"),
	ShootSkill			UMETA(DisplayName = "ShootSkill"),
	Sprint				UMETA(DisplayName = "Sprint"),
	Jump				UMETA(DisplayName = "Jump"),
};

USTRUCT()
struct FRGX_AbilityInitData
{
	GENERATED_BODY()

	// Ability to grant
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> Ability;
	UPROPERTY(EditDefaultsOnly)
	ERGX_AbilityInputID InputID = ERGX_AbilityInputID::None;
};