// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RGX_BTTask_CalculateActorFuturePosition.generated.h"

/**
 * 
 */

UCLASS()
class REGICIDEX_API URGX_BTTask_CalculateActorFuturePosition : public UBTTaskNode
{
	GENERATED_BODY()

public:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override; 

	/** initialize any asset related data */
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector PositionKey;

private:
	const AActor* GetTargetActor(UBehaviorTreeComponent& OwnerComp) const;
	FVector CalculateFuturePosition(UBehaviorTreeComponent& OwnerComp, const AActor* Target) const;
	bool SetPositionValue(UBehaviorTreeComponent& OwnerComp, const FVector& Position);

};


















