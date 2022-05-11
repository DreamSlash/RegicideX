#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RGX_CombatAssistComponent.generated.h"

class AActor;

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
	void EnableMovementVector();

	UFUNCTION(BlueprintCallable)
	void DisableMovementVector();

	UFUNCTION(BlueprintCallable)
	void AddMovementVector(FVector Direction, float Speed);

	UFUNCTION(BlueprintCallable)
	void RemoveMovementVector();

	UFUNCTION(BlueprintCallable)
	void SetAttackMoveDuration(float Duration);

protected:

	/** Attack Auto Assist */
	UPROPERTY(EditAnywhere)
	float AutoAssistRadius = 300.0f;

	UPROPERTY(EditAnywhere)
	float AutoAssistDot = 0.5f;

	UPROPERTY(EditAnywhere)
	float AutoAssistOffsetToEnemy = 200.0f;

	AActor* Target = nullptr;
	// ----------------------

	/** Attack Movement */
	UPROPERTY(EditAnywhere)
	float AttackOffsetToEnemy = 150.0f;

	UPROPERTY()
	FVector MoveVectorDirection = FVector(0.0f);

	float MoveVectorSpeed = 0.0f;

	float AutoAssistMove = 0.0f;

	UPROPERTY()
	bool bAddMoveVector = false;

	bool bMoveVectorEnabled = false;

	float AttackMoveDuration = 0.0f;

	float AttackMoveDurationLeft = 0.0f;
	// ------------------------------
};