// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_BTTaskDANewAttackMode.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type URGX_BTTaskDANewAttackMode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BB = AIController->GetBlackboardComponent();

	int AMode = BB->GetValueAsInt(TEXT("EnumNum"));

	int RandomNum = FMath::RandRange(MinAttacksNum, MaxAttacksNum);

	if(RandomNum == AMode)
	{
		int NewNum = RandomNum - 1;
		RandomNum = NewNum < MinAttacksNum ? RandomNum + 1 : NewNum;
	}

	BB->SetValueAsEnum(TEXT("AttackMode"), RandomNum);
	BB->SetValueAsInt(TEXT("EnumNum"), RandomNum);

	return EBTNodeResult::Succeeded;

}