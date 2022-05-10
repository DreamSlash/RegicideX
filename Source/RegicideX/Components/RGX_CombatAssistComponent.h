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
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable)
	void PerformAttackAutoAssist();

	UFUNCTION(BlueprintCallable)
	void AddMovementVector(FVector Direction, float Length);

	UFUNCTION(BlueprintCallable)
	void RemoveMovementVector();

protected:

	UPROPERTY()
	FVector MoveVectorDirection = FVector(0.0f);

	float MoveVectorLength = 0.0f;

	UPROPERTY()
	bool bAddMoveVector = false;
};