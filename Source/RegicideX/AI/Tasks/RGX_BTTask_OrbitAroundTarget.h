// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RGX_BTTask_OrbitAroundTarget.generated.h"

/**
 * 
 */
UCLASS()
class REGICIDEX_API URGX_BTTask_OrbitAroundTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector TargetCharacterKey;

	UPROPERTY(EditAnywhere)
	float OrbitMinTime = 0.0f;

	UPROPERTY(EditAnywhere)
	float OrbitMaxTime = 0.0f;

	// TODO: This node should be instanced
private:
	class AAIController* AIController = nullptr;
	class ARGX_EnemyBase* Enemy = nullptr;

	float OrbitDirection = 1.0f;
	float OrbitTime;
	float CurrentOrbitTime = 0.0f;

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

};
