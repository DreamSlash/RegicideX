// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "RGX_BTTask_FindStrafeLocation.generated.h"

/**
 * 
 */

UCLASS()
class REGICIDEX_API URGX_BT_FindStrafeLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	URGX_BT_FindStrafeLocation(const FObjectInitializer& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	UEnvQuery* LocationSeekerQuery;

	FEnvQueryRequest LocationSeekerQueryRequest;

	void LocationSeekerQueryFinished(TSharedPtr<FEnvQueryResult> Result);

	class ARGX_EnemyBaseController* Controller;

	FVector StrafeLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	float Distance = 500.0f;

	bool IsDistanceGreaterThanX(const FVector& Location);

};


















