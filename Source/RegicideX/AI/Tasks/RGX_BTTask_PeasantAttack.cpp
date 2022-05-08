// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_BTTask_PeasantAttack.h"
#include "AIController.h"
#include "AbilitySystemComponent.h"
#include "RegicideX/Actors/Enemies/RGX_Peasant.h"

URGX_BTTask_PeasantAttack::URGX_BTTask_PeasantAttack()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = true;
	NodeName = "Peasant Attack Task";
}

EBTNodeResult::Type URGX_BTTask_PeasantAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* const owner = OwnerComp.GetAIOwner();
	ARGX_Peasant* const self = Cast<ARGX_Peasant>(owner->GetPawn());
	if (!self)
		return EBTNodeResult::Failed;

	UAbilitySystemComponent* ASC = self->GetAbilitySystemComponent();
	if (ASC)
	{
		//ASC->TryActivateAbility()
	}

	FBTPeasantMeleeAttackMemory* memory = reinterpret_cast<FBTPeasantMeleeAttackMemory*>(NodeMemory);
	memory->bIsAttacking = true;
	memory->TimeToAttackFor = self->PunchMontage->GetPlayLength(); // Should AnimMontage be public?
	memory->TimeStartedAttack = GetWorld()->GetTimeSeconds();

	self->Attack();

	return EBTNodeResult::InProgress;
}

void URGX_BTTask_PeasantAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!OwnerComp.GetAIOwner())
		return;

	FBTPeasantMeleeAttackMemory* memory = reinterpret_cast<FBTPeasantMeleeAttackMemory*>(NodeMemory);

	ARGX_Peasant* const self = OwnerComp.GetAIOwner()->GetPawn<ARGX_Peasant>();

	// if attack duration is done, finish task ...
	const float TimeSeconds = GetWorld()->GetTimeSeconds() - memory->TimeStartedAttack;
	if (TimeSeconds >= memory->TimeToAttackFor && memory->bIsAttacking)
	{
		memory->bIsAttacking = false;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
