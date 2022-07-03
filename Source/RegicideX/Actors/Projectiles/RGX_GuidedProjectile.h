// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RegicideX/Actors/Projectiles/RGX_HitboxProjectile.h"
#include "RGX_GuidedProjectile.generated.h"

class URGX_HitboxComponent;

/**
 * 
 */
UCLASS()
class REGICIDEX_API ARGX_GuidedProjectile : public ARGX_HitboxProjectile
{
	GENERATED_BODY()
	
public:
	ARGX_GuidedProjectile();

protected:
	virtual void Tick(float DeltaTime) override;

protected:
	void RotateToTarget(float DeltaTime);
	void CheckDistance();
	void Move(float DeltaTime);

protected:
	AActor* TargetActor = nullptr;

	bool bStopFollowing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StopFollowingDistance = 200.f;
};
