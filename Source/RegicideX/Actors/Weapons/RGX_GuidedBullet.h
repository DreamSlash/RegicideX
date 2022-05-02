// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RGX_Bullet.h"
#include "RGX_GuidedBullet.generated.h"

/**
 * 
 */


UCLASS()
class REGICIDEX_API ARGX_GuidedBullet : public ARGX_Bullet
{

	GENERATED_BODY()

	bool StopFollowing = false;

public:
	ARGX_GuidedBullet();

	ARGX_GuidedBullet(AActor* Target);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float StopFollowingDistance = 200.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called every frame
	void Tick(float DeltaTime) override;

	void RotateToTarget(float DeltaTime);

};
