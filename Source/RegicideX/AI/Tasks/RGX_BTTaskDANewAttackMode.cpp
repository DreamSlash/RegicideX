// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_BTTaskDANewAttackMode.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type URGX_BTTaskDANewAttackMode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BB = AIController->GetBlackboardComponent();

	const int AMode = BB->GetValueAsEnum(TEXT("AttackMode"));

	int RandomNum = FMath::RandRange(MinAttacksNum, MaxAttacksNum);

	if(RandomNum == AMode)
	{
		RandomNum = RandomNum + 1 > MaxAttacksNum ? RandomNum + 1 : MinAttacksNum;
	}

	BB->SetValueAsEnum(TEXT("AttackMode"), RandomNum);

	return EBTNodeResult::Succeeded;

}