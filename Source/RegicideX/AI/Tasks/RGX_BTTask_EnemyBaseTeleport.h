// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "RGX_BTTask_EnemyBaseTeleport.generated.h"

UCLASS()
class REGICIDEX_API URGX_BTTask_EnemyBaseTeleport : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
