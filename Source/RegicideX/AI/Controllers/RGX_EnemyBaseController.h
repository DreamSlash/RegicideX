#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RGX_EnemyBaseController.generated.h"

UENUM(BlueprintType)
namespace ERGX_EnemyAIState
{
	enum Type
	{
		None		UMETA(DisplayName = "None"),
		Attacking	UMETA(DisplayName = "Attacking"),
		Holding		UMETA(DisplayName = "Holding"),
		Waiting		UMETA(DisplayName = "Waiting"),
		Recovering	UMETA(DisplayName = "Recovering")
	};
}

UENUM(BlueprintType)
namespace ERGX_StrafeDirection
{
	enum Type
	{
		None		UMETA(DisplayName = "None"),
		Forward		UMETA(DisplayName = "Forward"),
		Backward	UMETA(DisplayName = "Backward"),
		Right		UMETA(DisplayName = "Right"),
		Left		UMETA(DisplayName = "Left")
	};
}

UCLASS()
class REGICIDEX_API ARGX_EnemyBaseController : public AAIController
{
	GENERATED_BODY()

public:
	ARGX_EnemyBaseController(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;
	void OnPossess(APawn* InPawn) override;

public:
	UPROPERTY(transient)
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(transient)
	class UBlackboardComponent* BlackboardComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeConsecutiveHits = 1.0f;

	class ARGX_EnemyBase* Agent = nullptr;
	class ARGX_CombatManager* CombatManager = nullptr;

	// Blackboard Key Ids
	uint8 TargetKeyId;
	uint8 DistanceToPlayerKeyId;
	uint8 RandomNumberKeyId;
	uint8 AIStateKeyId;
	uint8 ConsecutiveHitsKeyId;
	uint8 StrafeDirectionKeyId;
	uint8 StrafeLocationKeyId;

public:
	UFUNCTION(BlueprintCallable)
		virtual void DamageTaken();

	UFUNCTION(BlueprintCallable)
		ERGX_EnemyAIState::Type GetEnemyAIState() const;
	UFUNCTION(BlueprintCallable)
		void SetEnemyAIState(ERGX_EnemyAIState::Type state);

	UFUNCTION(BlueprintCallable)
		void StartLogic();

protected:
	bool InitializeBlackboard(UBlackboardComponent& BlackboardComp, UBlackboardData& BlackboardAsset) override;

private:
	FTimerHandle TimerHandle;

};
