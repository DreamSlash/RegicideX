// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RGX_BTTask_EnemyStrafing.generated.h"

/**
 * 
 */

UCLASS()
class REGICIDEX_API URGX_BT_EnemyStrafing : public UBTTaskNode
{
	GENERATED_BODY()

public:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override; 

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	/** initialize any asset related data */
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector DirectionKey;

private:
	FVector GetDirection(UBehaviorTreeComponent& OwnerComp) const;

	FVector Direction = FVector::ZeroVector;

};


















