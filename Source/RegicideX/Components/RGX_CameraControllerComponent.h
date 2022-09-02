#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include <functional>
#include "RGX_CameraControllerComponent.generated.h"

class ARGX_EnemyBase;

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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ToggleTargeting();

	void EnableTargeting();
	void DisableTargeting();

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

private:
	TWeakObjectPtr<ARGX_EnemyBase> CurrentTarget;

	float OriginalArmLength;

	bool bIsActive = false;

};
