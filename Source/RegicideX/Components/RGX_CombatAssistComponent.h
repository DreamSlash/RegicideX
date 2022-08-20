#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RGX_CombatAssistComponent.generated.h"

class AActor;
class ARGX_EnemyBase;

USTRUCT()
struct FRGX_ActorAngle
{
	GENERATED_BODY();

public:

	AActor* Actor = nullptr;
	float Angle;
};

FORCEINLINE bool operator< (const FRGX_ActorAngle& lhs, const FRGX_ActorAngle& rhs)
{
	return lhs.Angle < rhs.Angle;
}
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRGX_TargetUpdatedDelegate, const ARGX_EnemyBase*, Enemy);

UCLASS(meta = (BlueprintSpawnableComponent))
class REGICIDEX_API URGX_CombatAssistComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URGX_CombatAssistComponent();

	void BeginPlay() override;
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable)
	void PerformAttackAutoAssist();

	UFUNCTION(BlueprintCallable)
	void EnableMovementVector();

	UFUNCTION(BlueprintCallable)
	void DisableMovementVector();

	UFUNCTION(BlueprintCallable)
	void AddMovementVector(const FVector Direction, const float Speed, const bool bNewIsAttacking);

	UFUNCTION(BlueprintCallable)
	void RemoveMovementVector();

	UFUNCTION(BlueprintCallable)
	void SetAttackMoveDuration(float Duration);

	UPROPERTY(BlueprintAssignable)
	FRGX_TargetUpdatedDelegate OnTargetUpdated;

protected:

	void UpdateTarget();

	TArray<AActor*> GetClosestEnemiesInRange(const float Range, const bool bSameFallingState) const;
	ARGX_EnemyBase* GetFrontEnemy(const TArray<AActor*>& Enemies, const float AngleDiscardThreshHold = 180.0f);
	void SetNewTarget(ARGX_EnemyBase* NewTarget);

protected:

	/** Auto Assist Targetting */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AutoAssistCloseRadius = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AutoAssistMaxRadius = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxRadiusAngleDiscard = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TargettingZTopOffset = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TargettingZBottomOffset = 50.0f;
	
	/* Enemies at that angle from player's forward will be checked against the camera frustum to discard
	the ones outside */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CheckCameraAngle = 30.0f;

	uint32 NumEnemiesInsideFrustum = 0;
	// --------------------------

	/** Auto Assit Attack */
	UPROPERTY(EditAnywhere)
	float AutoAssistDot = 0.5f;

	UPROPERTY(EditAnywhere)
	float AutoAssistOffsetToEnemy = 200.0f;

	UPROPERTY(EditAnywhere)
	float MaxAutoassistMove = 200.0f;
	// ---------------------
	ARGX_EnemyBase* Target = nullptr;
	// ----------------------

	/** Attack Movement */
	UPROPERTY(EditAnywhere)
	float AttackOffsetToEnemy = 150.0f;

	UPROPERTY()
	FVector MoveVectorDirection = FVector(0.0f);

	float MoveVectorSpeed = 0.0f;

	float AutoAssistMove = 0.0f;

	bool bIsAttacking = false;

	UPROPERTY()
	bool bAddMoveVector = false;

	bool bMoveVectorEnabled = false;

	float AttackMoveDuration = 0.0f;

	float AttackMoveDurationLeft = 0.0f;
	// ------------------------------
};