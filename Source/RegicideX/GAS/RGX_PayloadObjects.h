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
	FVector LaunchForce;

public:
	URGX_LaunchEventPayload();

};