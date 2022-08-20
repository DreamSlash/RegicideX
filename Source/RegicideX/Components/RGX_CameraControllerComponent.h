#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ToggleTargetting();

	UFUNCTION(BlueprintCallable)
	void SetTarget(const ARGX_EnemyBase* NewTarget);

private:
	TWeakObjectPtr<const ARGX_EnemyBase> CurrentTarget;

	float OriginalArmLength;

};
