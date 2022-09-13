#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "RGX_EngineSubsystem.generated.h"

/**
 *
 */
UCLASS()
class REGICIDEX_API URGX_EngineSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	void Initialize(FSubsystemCollectionBase& Collection) override;
};
