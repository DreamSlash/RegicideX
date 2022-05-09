#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RGX_LaserBeamWeapon.generated.h"

class USplineComponent;
class USplineMeshComponent;
class UStaticMeshComponent;
class UStaticMesh;
class UMaterial;
class UParticleSystemComponent;

class UGameplayEffect;

UCLASS()
class REGICIDEX_API ARGX_LaserBeamWeapon : public AActor
{
	GENERATED_BODY()

	bool FollowTarget = true;
	bool bHittingTarget = false;

public:	
	// Sets default values for this actor's properties
	ARGX_LaserBeamWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector SourcePoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* TargetActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* OwnerActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* EndPointMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystemComponent* EndPointParticle = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USplineComponent* PathSplineComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<USplineMeshComponent*> PathSplineMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RaySpeed = 700.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ForgetDistance = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ForgetTime = 1.0f;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UGameplayEffect> EffectToApply;

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

public:	

	UFUNCTION()
		void ComputeNewEndpoint(float DeltaTime);

	UFUNCTION()
		void MoveAndDrawRay(float DeltaTime);

	UFUNCTION(BlueprintImplementableEvent)
		void SpawnSplineMesh();

	UFUNCTION()
		void SetSourcePoint(FVector SP);

	UFUNCTION()
		void SetOwnerActor(AActor* OA);

	UFUNCTION()
		void ApplyEffect(AActor* OtherActor);

	UFUNCTION()
		void CheckRayTraces(FVector& NewLocation, float DeltaTime);

	void CheckDistance();
};













