// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "RGX_EnemyBase.h"
#include "RGX_Peasant.generated.h"

class UBehaviorTree;
class ARGX_GroupManager;

/**
 * 
 */
UCLASS()
class REGICIDEX_API ARGX_Peasant : public ARGX_EnemyBase
{
	GENERATED_BODY()
public:

	// Setting default values
	ARGX_Peasant();

	UPROPERTY(EditAnywhere)
	UBehaviorTree* BTree;

	// Player target
	UPROPERTY(EditDefaultsOnly)
	AActor* TargetActor;

	UPROPERTY(EditAnywhere)
	ARGX_GroupManager* manager;

	// String to show the status in the Text RenderComponent
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString TextStatusString;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bWasHit = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bOnAir = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bInCombat = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector IdlePosition;

	//virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Idle();
	void Attack();

	float GetDistanceToTarget();

protected:

	// Animation Montages ...
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* PunchMontage;

	UPROPERTY(EditDefaultsOnly);
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly);
	UAnimMontage* ReceiveDamageMontage;

	virtual void BeginPlay() override;
	//virtual void EndPlay(Const EEndPlayReason::Type EndPlayReason) override;

private:

	void RotateToTarget();
};
