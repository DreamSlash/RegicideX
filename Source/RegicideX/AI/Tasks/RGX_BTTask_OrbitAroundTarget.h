// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "RGX_BTTask_OrbitAroundTarget.generated.h"

/*
struct FBT_OrbitAroundTargetMemory 
{
	TWeakObjectPtr<class AAIController> AIController = nullptr;
	TWeakObjectPtr<class ARGX_EnemyBase> Enemy = nullptr;

	float OrbitDirection = 1.0f;
	float OrbitTime;
	float CurrentOrbitTime = 0.0f;
};
*/
/**
 * 
 */

UCLASS()
class REGICIDEX_API URGX_BTTask_OrbitAroundTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	// TODO: La guarrada padre. Should use node instances or node memory
	const FName OrbitMinTimeName = "OrbitMinTime";
	const FName OrbitMaxTimeName = "OrbitMaxTime";
	const FName OrbitTimeName = "OrbitTime";
	const FName OrbitDirectionName = "OrbitDirection";
	const FName CurrentOrbitTimeName = "CurrentOrbitTime";
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
};
