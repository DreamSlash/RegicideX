// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RGX_EnemyBase.h"
#include "RGX_MeleeAngel.generated.h"

class URGX_MovementAssistComponent;
class URGX_HitboxComponent;
class USphereComponent;


UCLASS()
class REGICIDEX_API ARGX_MeleeAngel : public ARGX_EnemyBase
{
	GENERATED_BODY()
	
public:

	ARGX_MeleeAngel();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAtFloor = false;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCharging = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTornadoActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URGX_MovementAssistComponent* MovementAssistComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector DivineDescentTargetLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector HeightGoal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URGX_HitboxComponent* BHHitboxComponent = nullptr;

	UPROPERTY(EditAnywhere)
	USphereComponent* TornadoSphereCollider = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FRGX_EffectContextContainer> TornadoEffectsToApply;

protected:
	FVector ChargeVelocity;


public:
	void RotateToTarget(float DeltaTime) override;

	void SetGravityScale(float value);

	void Tick(float DeltaTime) override;

	FVector GetVelocity() const;

	void SetChargeVelocity(const FVector NewChargeVelocity) { ChargeVelocity = NewChargeVelocity; }

	UFUNCTION(BlueprintCallable)
	void ApplyTornadoEffects(AActor* OtherActor);

	UFUNCTION(BlueprintImplementableEvent)
	void SetTornadoFXVisibility(bool Visible);
};
