#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include <functional>
#include "RGX_CameraControllerComponent.generated.h"

class ARGX_EnemyBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRGX_TargetUpdatedDelegate, ARGX_EnemyBase*, Enemy);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class REGICIDEX_API URGX_CameraControllerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URGX_CameraControllerComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float TargetTraceDistance = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PitchMaxAngle = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float YawhMaxAngle = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PitchDesiredAngle = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float YaxDesiredAngle = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TargetingRange = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TargetingConeAngle = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxZoomOut = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ZoomOutDistancePerUnseenGroup = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float GroupNumEnemies = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CameraSpeed = 5.0f;

	UPROPERTY(BlueprintAssignable)
		FRGX_TargetUpdatedDelegate OnTargetUpdated;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void EnableTargeting();
	void DisableTargeting();

	void CheckYawInput(float Rate);

	void TargetLeft();
	void TargetRight();

	UFUNCTION(BlueprintCallable)
	void SetTarget(ARGX_EnemyBase* NewTarget);

private:
	void FindTarget();
	void FindTargetUsingSphere();
	void FindNearestTargetUsingSphere(bool RightDirection);

	TArray<AActor*> GetNearbyActorsUsingSphere(const TArray<AActor*>& IgnoredActors) const;
	float CalculateDotProduct(const FVector& SourceLocation, const FVector& SourceDir, const AActor* Actor) const;

	float CalculateDesiredDistance();
	FRotator CalculateDesiredRotation(float DesiredDistance);

private:
	TWeakObjectPtr<ARGX_EnemyBase> CurrentTarget;

	float OriginalArmLength;

	bool bIsActive = false;
	bool bFindNearestTargetExecuted = false;

};
