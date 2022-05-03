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

UCLASS()
class REGICIDEX_API ARGX_LaserBeamWeapon : public AActor
{
	GENERATED_BODY()

	bool FollowTarget;


public:	
	// Sets default values for this actor's properties
	ARGX_LaserBeamWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector SourcePoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* EndPointMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystemComponent* EndPointParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USplineComponent* PathSplineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USplineMeshComponent* PathSplineMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RaySpeed = 700.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ForgetDistance = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterial* Material;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UFUNCTION()
		void ComputeNewEndpoint(float DeltaTime);

	UFUNCTION()
		void MoveRay(float DeltaTime);

	UFUNCTION(BlueprintImplementableEvent)
		void SpawnSplineMesh();

	UFUNCTION()
		void SetSourcePoint(FVector SP);

};













