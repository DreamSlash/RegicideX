// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "RGX_EnemyBase.h"
#include "RGX_Peasant.generated.h"

class URGX_MovementAssistComponent;
class UBehaviorTree;
class ARGX_GroupManager;

UCLASS()
class REGICIDEX_API ARGX_Peasant : public ARGX_EnemyBase
{
	GENERATED_BODY()
public:

	ARGX_Peasant();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URGX_MovementAssistComponent* MovementAssistComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNiagaraComponent* TellVFX = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int IdleAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TellVFXTime = 1.0f;

public:
	float GetDistanceToTarget() const;

	UFUNCTION(BlueprintCallable)
	void ActivateTellVFX();

protected:

	UPROPERTY()
	FVector WanderingPoint;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void Activate() override;
	virtual void Deactivate() override;

	// Bool to signal if actor is going to get destroyed.
	virtual void HandleDeath() override;

private:
	FTimerHandle TellVFXTimerHandle;

};
