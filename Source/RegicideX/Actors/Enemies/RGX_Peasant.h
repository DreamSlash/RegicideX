// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "RGX_EnemyBase.h"
#include "RGX_Peasant.generated.h"

class UBehaviorTree;
class ARGX_GroupManager;

UCLASS()
class REGICIDEX_API ARGX_Peasant : public ARGX_EnemyBase
{
	GENERATED_BODY()
public:

	ARGX_Peasant();

	UPROPERTY(EditAnywhere)
	UBehaviorTree* BTree = nullptr;

	// String to show the status in the Text RenderComponent
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString TextStatusString;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bInCombat = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int IdleAction;

	UPROPERTY(EditDefaultsOnly)
	float WanderSpeed = 100.0f;

	UPROPERTY(EditDefaultsOnly)
	float MaxSpeed = 400.0f;

	float GetDistanceToTarget() const;

	// Bool to signal if actor is going to get destroyed.
	bool ToBeDestroyed = false;
	void HandleDeath() override;
	void DestroyPeasant();

protected:

	UPROPERTY()
	FVector WanderingPoint;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
};
