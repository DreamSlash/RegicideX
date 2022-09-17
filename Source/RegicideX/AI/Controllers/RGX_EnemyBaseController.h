#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RGX_EnemyBaseController.generated.h"

UCLASS()
class REGICIDEX_API ARGX_EnemyBaseController : public AAIController
{
	GENERATED_BODY()

public:
	ARGX_EnemyBaseController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeConsecutiveHits = 1.0f;

	UFUNCTION(BlueprintCallable)
	virtual void DamageTaken();

	UFUNCTION(BlueprintCallable)
	ERGX_EnemyAIState GetEnemyAIState() const;
	UFUNCTION(BlueprintCallable)
	void SetEnemyAIState(ERGX_EnemyAIState state);

protected:
	bool InitializeBlackboard(UBlackboardComponent& BlackboardComp, UBlackboardData& BlackboardAsset) override;

private:
	FTimerHandle TimerHandle;

};
