// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RGX_EnemyBase.h"
#include "RGX_MeleeAngel.generated.h"

class URGX_MovementAssistComponent;




UCLASS()
class REGICIDEX_API ARGX_MeleeAngel : public ARGX_EnemyBase
{
	GENERATED_BODY()
	
public:

	ARGX_MeleeAngel();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLevitating = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bFlying = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCharging = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSweeping = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URGX_MovementAssistComponent* MovementAssistComponent;

protected:
	FVector ChargeVelocity;

public:
	void RotateToTarget(float DeltaTime) override;

	void SetGravityScale(float value);

	void Tick(float DeltaTime) override;

	FVector GetVelocity() const;

	void SetChargeVelocity(const FVector NewChargeVelocity) { ChargeVelocity = NewChargeVelocity; }
};
