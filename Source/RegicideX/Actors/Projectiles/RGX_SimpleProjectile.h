// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RegicideX/Actors/Projectiles/RGX_HitboxProjectile.h"
#include "RGX_SimpleProjectile.generated.h"

class UProjectileMovementComponent;

/**
 * Simple Projectile updated with Projectile Movement Component
 */
UCLASS()
class REGICIDEX_API ARGX_SimpleProjectile : public ARGX_HitboxProjectile
{
	GENERATED_BODY()

public:
	ARGX_SimpleProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovementComponent = nullptr;

protected:
	virtual void BeginPlay() override;
};
