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

	UPROPERTY()
		class USpringArmComponent* SpringArm;

	UPROPERTY()
		class UCameraComponent* Camera;

	UPROPERTY(Category = DynamicCamera, EditAnywhere, BlueprintReadWrite)
		float CombatRadius = 1200;

	UPROPERTY(Category = DynamicCamera, EditAnywhere, BlueprintReadWrite)
		float MaxZoomOut = 2000.0f;

	UPROPERTY(Category = DynamicCamera, EditAnywhere, BlueprintReadWrite)
		float ZoomOutPerVisibleEnemy = 50.0f;

	UPROPERTY(Category = DynamicCamera, EditAnywhere, BlueprintReadWrite)
		float ZoomOutPerNotVisibleEnemy = 300.0f;

	UPROPERTY(Category = DynamicCamera, EditAnywhere, BlueprintReadWrite)
		float ZoomOutPerAngelNearby = 300.0f;

	UPROPERTY(Category = DynamicCamera, EditAnywhere, BlueprintReadWrite)
		float ZoomOutAirCombo = 200.0f;

	UPROPERTY(Category = DynamicCamera, EditAnywhere, BlueprintReadWrite)
		float CameraSpeed = 1.0f;

	UPROPERTY(Category = DynamicCamera, EditAnywhere, BlueprintReadWrite)
		float UpdateDistanceOffset = 10.0f;

	UPROPERTY(Category = TargetingSystem, EditAnywhere, BlueprintReadWrite)
		float TargetingConeAngle = 45.0f;

	UPROPERTY(Category = TargetingSystem, EditAnywhere, BlueprintReadWrite)
		float DesiredYawAngle = 20.0f;

	UPROPERTY(Category = TargetingSystem, EditAnywhere, BlueprintReadWrite)
		float MinYawAngle = 15.0f;

	UPROPERTY(Category = TargetingSystem, EditAnywhere, BlueprintReadWrite)
		float MaxYawAngle = 30.0f;

	UPROPERTY(Category = TargetingSystem, EditAnywhere, BlueprintReadWrite)
		float DesiredPitchAngle = 0.0f;

	UPROPERTY(Category = TargetingSystem, EditAnywhere, BlueprintReadWrite)
		float MinPitchAngle = -10.0f;

	UPROPERTY(Category = TargetingSystem, EditAnywhere, BlueprintReadWrite)
		float MaxPitchAngle = 10.0f;

	UPROPERTY(BlueprintAssignable)
		FRGX_TargetUpdatedDelegate OnTargetUpdated;

	UPROPERTY()
		bool bTargetingEnabled = false;

	UPROPERTY()
		TWeakObjectPtr<ARGX_EnemyBase> CurrentTarget;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ToggleTargeting();
	void EnableTargeting();
	void DisableTargeting();

	void CheckYawInput(float Rate);

	void TargetLeft();
	void TargetRight();

	UFUNCTION(BlueprintCallable)
	void SetTarget(ARGX_EnemyBase* NewTarget);

private:
	void FindTarget();
	void FindTarget(TArray<AActor*>& Targets);
	void FindNearestTargetUsingSphere(bool RightDirection);

	void CalculateSpringArmDistance(const TArray<AActor*>& Targets, float DeltaTime);
	float CalculateSpringArmDistanceByEnemies(const TArray<AActor*>& Targets) const;

	void UpdateTargeting(TArray<AActor*>& Targets, float DeltaTime);

	FRotator CalculateDesiredRotation() const;
	float CalculateDesiredAngle(float DesiredAngle) const;
	float CalculateFinalYaw(float DesiredYaw) const;
	float CalculateFinalPitch(float DesiredPitch) const;

	TArray<AActor*> GetNearbyActorsUsingSphere(const TArray<AActor*>& IgnoredActors) const;
	float CalculateDotProduct(const FVector& SourceLocation, const FVector& SourceDir, const AActor* Actor) const;

private:
	UPROPERTY()
	class ARGX_PlayerCharacter* Owner = nullptr;

	float OriginalArmLength;

	bool bFindNearestTargetExecuted = false;

};
