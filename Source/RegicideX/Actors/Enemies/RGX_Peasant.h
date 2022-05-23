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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ARGX_GroupManager* manager = nullptr;

	// String to show the status in the Text RenderComponent
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString TextStatusString;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bWasHit = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bOnAir = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bInCombat = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bAttacking = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int IdleAction;

	UPROPERTY(EditDefaultsOnly)
	float WanderSpeed = 100.0f;

	UPROPERTY(EditDefaultsOnly)
	float MaxSpeed = 400.0f;

	UFUNCTION(BlueprintCallable)
	void ResetAttacking();

	float GetDistanceToTarget() const;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* IdleMontage = nullptr;

	// Bool to signal if actor is going to get destroyed.
	bool ToBeDestroyed = false;
	FTimerHandle CorpseTimerHandle;
	void DestroyPeasant();

protected:

	UPROPERTY()
	FVector WanderingPoint;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
};
