#pragma once

#include "CoreMinimal.h"
#include "RegicideX/Actors/Projectiles/RGX_HitboxProjectile.h"
#include "Enemies/RGX_EnemyBase.h"
#include "RGX_SpearProjectile.generated.h"

class USceneComponent;
class URGX_HitboxComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

UCLASS(Blueprintable, Abstract)
class ARGX_SpearProjectile : public ARGX_HitboxProjectile
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* RootScene;

public:

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProjectileMovementComponent* ProjectileMovementComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	int RemainingSeconds = 5;

	UPROPERTY(EditDefaultsOnly)
	float TargetMaxRange = INFINITY;// 500.0f;

	UPROPERTY()
	float Angle;

	UPROPERTY(BlueprintReadOnly)
	int SpawnIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DelayTime = 0.0f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ARGX_EnemyBase> EnemyClass;

protected:

	UPROPERTY(EditDefaultsOnly)
	float DistanceFromCaster = 100.0f;

	UPROPERTY()
	bool bWasLaunched = false;

	UPROPERTY(BlueprintReadOnly)
	FVector LaunchVelocity = FVector(0, 0, 0);

	UPROPERTY()
	ARGX_EnemyBase* ClosestEnemy = nullptr;

public:
	ARGX_SpearProjectile();

	void BeginPlay() override;

	void Tick(float DeltaTime) override;

	void LaunchProjectile(const AActor* target = nullptr);

	UFUNCTION(BlueprintNativeEvent)
	void HandleHit(AActor* OtherActor);

protected:

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpule, const FHitResult& Hit);

	bool IsHostile(const IGameplayTagAssetInterface* InstigatorTagInterface, const IGameplayTagAssetInterface* OtherTagInterface) const;
};