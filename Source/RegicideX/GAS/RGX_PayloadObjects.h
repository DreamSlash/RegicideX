#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "GameplayEffect.h"
#include "RGX_PayloadObjects.generated.h"

UCLASS()
class REGICIDEX_API URGX_LaunchEventPayload : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY()
	float LaunchHorizontalForce;

	UPROPERTY()
	float LaunchVerticalForce;

	UPROPERTY()
	bool bOverrideHorizontal = false;

	UPROPERTY()
	bool bOverrideVertical = false;

	UPROPERTY()
	FVector ForceOrigin;

public:
	URGX_LaunchEventPayload();

};

UCLASS()
class URGX_RGXEventDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	FGameplayTag EventTag;

public:
	FGameplayTag GetEventTag() { return EventTag; }
};

USTRUCT(BlueprintType)
struct FRGX_EffectContextContainer
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> EffectToApply;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	URGX_RGXEventDataAsset* Payload = nullptr;
};

USTRUCT()
struct FRGX_AbilityEffectsInfo
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	TArray<URGX_RGXEventDataAsset*> GameplayEventsToTarget;

	UPROPERTY()
	TArray<URGX_RGXEventDataAsset*> GameplayEventsToOwner;

	UPROPERTY()
	TArray<TSubclassOf<UGameplayEffect>> GameplayEffectsToTarget;

	UPROPERTY()
	TArray<TSubclassOf<UGameplayEffect>> GameplayEffectsToOwner;
};

UENUM()
enum class ERGX_AnimEvent: uint8
{
	None				UMETA(DisplayName = "None"),
	BasicHitReact		UMETA(DisplayName = "BasicHitReact"),
	KnockbackHitReact	UMETA(DisplayName = "KnockbackHitReact"),
	LaunchHitReact		UMETA(DisplayName = "LaunchHitReact"),
	AirHitReact			UMETA(DisplayName = "AirHitReact"),
	Death				UMETA(DisplayName = "Death")
};

UCLASS(BlueprintType)
class URGX_DamageEventDataAsset : public URGX_RGXEventDataAsset
{
	GENERATED_BODY()

public:
	URGX_DamageEventDataAsset();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName DamageCurveName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName AttributeScalingCurveName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveTable* DamageLevelCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ERGX_AnimEvent HitReactFlag = ERGX_AnimEvent::None;
};

UCLASS(BlueprintType)
class URGX_LaunchEventDataAsset : public URGX_RGXEventDataAsset
{
	GENERATED_BODY()

public:
	URGX_LaunchEventDataAsset();

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LaunchHorizontalForce;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LaunchVerticalForce;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bOverrideHorizontal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bOverrideVertical;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bInstigatorOrigin;
};

UCLASS(BlueprintType)
class URGX_KnockdownEventDataAsset : public URGX_RGXEventDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY()
	float KnockdownDuration;

	/**The rate at which the knockdown animation should be played. Stronger knockdowns should have higher multipliers.*/
	UPROPERTY()
	float KnockdownAnimationSpeedMultiplier;
};

UCLASS(BlueprintType)
class URGX_DialogueDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FText> Dialogue;
};