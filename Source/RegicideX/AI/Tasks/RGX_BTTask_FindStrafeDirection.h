// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "RGX_BTTask_FindStrafeDirection.generated.h"

/**
 * 
 */

UCLASS()
class REGICIDEX_API URGX_BT_FindStrafeDirection : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	URGX_BT_FindStrafeDirection(const FObjectInitializer& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override; 

	void InitializeFromAsset(UBehaviorTree& Asset) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	UEnvQuery* LocationSeekerQuery;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector DistanceKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	float Distance = 500.0f;

private:
	void LocationSeekerQueryFinished(TSharedPtr<FEnvQueryResult> Result);

	bool IsDistanceGreaterThanX(const FVector& Location);

	float GetDistance(UBehaviorTreeComponent& OwnerComp) const;

private:
	FEnvQueryRequest LocationSeekerQueryRequest;

	class ARGX_EnemyBaseController* Controller;

	FVector StrafeLocation = FVector::ZeroVector;

};


















