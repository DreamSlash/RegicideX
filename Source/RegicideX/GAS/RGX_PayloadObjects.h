#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

USTRUCT()
struct FRGX_AbilityEffectsInfo
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TArray<FGameplayTag> EventToTargetTags;

	UPROPERTY()
	TArray<UPrimaryDataAsset*> GameplayEventsToTarget;

	UPROPERTY()
	TArray<FGameplayTag> EventToOwnerTags;

	UPROPERTY()
	TArray<UPrimaryDataAsset*> GameplayEventsToOwner;

	UPROPERTY()
	TArray<TSubclassOf<UGameplayEffect>> GameplayEffectsToTarget;

	UPROPERTY()
	TArray<TSubclassOf<UGameplayEffect>> GameplayEffectsToOwner;
};

UCLASS(BlueprintType)
class URGX_LaunchEventDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LaunchHorizontalForce;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LaunchVerticalForce;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bOverrideHorizontal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bOverrideVertical;
};