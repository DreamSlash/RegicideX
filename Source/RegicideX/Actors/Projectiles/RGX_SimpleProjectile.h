// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RegicideX/Actors/Projectiles/RGX_Projectile.h"
#include "RGX_SimpleProjectile.generated.h"

class UProjectileMovementComponent;
class URGX_HitboxComponent;

/**
 * Simple Projectile updated with Projectile Movement Component
 */
UCLASS()
class REGICIDEX_API ARGX_SimpleProjectile : public ARGX_Projectile
{
	GENERATED_BODY()

public:
	ARGX_SimpleProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	URGX_HitboxComponent* HitboxComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovementComponent = nullptr;

protected:
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	virtual void OnHitboxOverlap(AActor* OverlappedActor);
};
