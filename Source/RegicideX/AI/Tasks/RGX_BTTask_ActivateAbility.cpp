// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_BTTask_ActivateAbility.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"

URGX_BTTask_ActivateAbility::URGX_BTTask_ActivateAbility()
{
	bCreateNodeInstance = true;

}

EBTNodeResult::Type URGX_BTTask_ActivateAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = EBTNodeResult::Failed;

	const AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* Pawn = AIController->GetPawn();

	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn);
	if (ASC->TryActivateAbilityByClass(AbilityToActivte))
	{
		const FGameplayAbilitySpec* AbilitySpec = ASC->FindAbilitySpecFromClass(AbilityToActivte);

		UGameplayAbility* AbilityInstance = AbilitySpec->GetPrimaryInstance();
		if (AbilityInstance->IsActive())
		{
			AbilityInstance->OnGameplayAbilityEnded.AddUObject(this, &URGX_BTTask_ActivateAbility::OnAbilityEnded);
			Result = EBTNodeResult::InProgress;
		}
		else
		{
			Result = EBTNodeResult::Succeeded;
		}
	}

	return Result;
}

void URGX_BTTask_ActivateAbility::OnAbilityEnded(UGameplayAbility* EndedAbility)
{
	UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(GetOuter());
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}