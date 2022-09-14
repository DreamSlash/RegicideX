// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RGX_MovementAssistComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REGICIDEX_API URGX_MovementAssistComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URGX_MovementAssistComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ForwardMagnitude = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed = 1.0f;

private:

	FVector GoalPoint = FVector(0.0f);

	FVector Direction = FVector(0.0f);

	float DistanceToGoalPoint = 0.0f;

	bool bIsMoving = false;

protected:
	// Called when the game starts
	void BeginPlay() override;

	void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	void Move(float DeltaTime);

	bool CheckDistanceToGoalPoint();

public:	
	// Called every frame
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetDirection(const FVector NewDirection);

	UFUNCTION(BlueprintCallable)
	void SetMagnitudeAndSpeed(float Magnitude, float Speed);

	UFUNCTION(BlueprintCallable)
	void EnableMovementAssist();

	UFUNCTION(BlueprintCallable)
	void DisableMovementAssist();
};

DECLARE_LOG_CATEGORY_EXTERN(LogMoveAssist, Log, All);