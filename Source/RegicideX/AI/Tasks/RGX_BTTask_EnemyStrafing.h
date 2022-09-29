// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RegicideX/AI/Controllers/RGX_EnemyBaseController.h"
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
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	/** initialize any asset related data */
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

protected:
	UPROPERTY(EditAnywhere, Category = Input)
	TEnumAsByte<ERGX_StrafeDirection::Type> StrafeDirection = ERGX_StrafeDirection::None;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector DirectionKey;

private:
	ERGX_StrafeDirection::Type GetDirection(UBehaviorTreeComponent& OwnerComp) const;

private:
	ERGX_StrafeDirection::Type Direction = ERGX_StrafeDirection::None;

};


















