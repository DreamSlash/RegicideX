// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "RGX_SlowWall.generated.h"

class UBoxComponent;
class UCurveFloat;
class UGameplayEffect;

UCLASS()
class REGICIDEX_API ARGX_SlowWall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARGX_SlowWall();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* WallCollider = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LifeTimeOnceIsRisen = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* WallCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> WallEffect;

public:
	void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

public:
	UFUNCTION()
	void SlowActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	FTimerHandle DestroyTimerHandle;
	FTimeline RiseTimeLine;

	float InitialZ = 0.0f;

};
