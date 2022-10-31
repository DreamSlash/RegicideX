// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "RGX_MoveForwardAbilityTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMoveForwardDelegate);

/**
 * 
 */
UCLASS()
class REGICIDEX_API URGX_MoveForwardAbilityTask : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FMoveForwardDelegate OnFinished;

	UPROPERTY(BlueprintAssignable)
	FMoveForwardDelegate OnOverlapped;

	URGX_MoveForwardAbilityTask(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static URGX_MoveForwardAbilityTask* MoveForwardAbilityTask(UGameplayAbility* OwningAbility, FName TaskInstanceName, float Duration, FGameplayTag HitboxTag);

	void Activate() override;
	void TickTask(float DeltaTime) override;
	void OnDestroy(bool AbilityIsEnding) override;

private:
	UFUNCTION()
	void OnHitboxOverlap(AActor* OverlappedActor);

private:
	bool bEndTask = false;

	float Duration = 3.0f;

	FGameplayTag HitboxTag;

	FTimerHandle TimerHandle;

};
