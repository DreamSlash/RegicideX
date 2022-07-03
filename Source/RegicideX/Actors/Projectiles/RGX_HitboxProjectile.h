// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RegicideX/Actors/Projectiles/RGX_Projectile.h"
#include "RGX_HitboxProjectile.generated.h"

class URGX_HitboxComponent;

/**
 * 
 */
UCLASS()
class REGICIDEX_API ARGX_HitboxProjectile : public ARGX_Projectile
{
	GENERATED_BODY()

public:
	ARGX_HitboxProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	URGX_HitboxComponent* HitboxComponent = nullptr;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

protected:
	UFUNCTION()
	virtual void OnHitboxOverlap(AActor* OverlappedActor);
};
