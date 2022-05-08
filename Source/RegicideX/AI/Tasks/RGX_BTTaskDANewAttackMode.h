// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RGX_BTTaskDANewAttackMode.generated.h"

/**
 * 
 */


UCLASS()
class REGICIDEX_API URGX_BTTaskDANewAttackMode : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MinAttacksNum = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxAttacksNum = 4;

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};


