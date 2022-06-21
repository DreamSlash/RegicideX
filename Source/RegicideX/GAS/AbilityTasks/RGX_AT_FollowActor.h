// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "RGX_AT_FollowActor.generated.h"

class AActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFollowActorDelegate);

/**
 * 
 */
UCLASS()
class REGICIDEX_API URGX_AT_FollowActor : public UAbilityTask
{
	GENERATED_BODY()
	
public:

	URGX_AT_FollowActor();

	void InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static URGX_AT_FollowActor* FollowActor(UGameplayAbility* OwningAbility, FName TaskInstanceName, AActor* ActorToFollow, float Speed, float FollowDuration);

	void Activate() override;

	/** Tick function for this task, if bTickingTask == true */
	void TickTask(float DeltaTime) override;

	void OnDestroy(bool AbilityIsEnding) override;

public:
	UPROPERTY(BlueprintAssignable)
	FFollowActorDelegate	OnFollowActorEnded;

protected:

	bool bIsFinished;

	AActor* ActorToFollow = nullptr;

	float FollowSpeed;
	float FollowDuration;

	float TimeFollowStarted;
	float TimeFollowWillEnd;
};
