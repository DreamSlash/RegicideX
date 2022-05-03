// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RGX_BTTask_PeasantAttack.generated.h"


struct FBTPeasantMeleeAttack
{
	float TimeStartedAttack = 0.f;
	float TimeToAttackFor = 0.f;

	bool bIsAttacking = false;
	bool bFinishedAttacking = false;
	bool bHasStartedAttacking = false;
};

UCLASS()
class REGICIDEX_API URGX_BTTask_PeasantAttack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	URGX_BTTask_PeasantAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
