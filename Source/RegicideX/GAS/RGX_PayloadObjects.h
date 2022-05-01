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