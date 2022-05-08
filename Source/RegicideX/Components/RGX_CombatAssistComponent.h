#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RGX_CombatAssistComponent.generated.h"

UCLASS(meta = (BlueprintSpawnableComponent))
class REGICIDEX_API URGX_CombatAssistComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URGX_CombatAssistComponent();

	void BeginPlay() override;
	void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable)
	void PerformAttackAutoAssist();
};