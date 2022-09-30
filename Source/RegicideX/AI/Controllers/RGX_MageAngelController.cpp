// Fill out your copyright notice in the Description page of Project Settings.

#include "RGX_MageAngelController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"

#pragma optimize("", off)

void ARGX_MageAngelController::OnMageAngelHealthChanged(float CurrentHealth, float MaxHealth)
{
	const float proportion = CurrentHealth / MaxHealth;

	switch (MageAngelPhase)
	{
	case ERGX_MageAngelPhase::Phase1:
		{
			if (proportion < Phase2Threshold)
			{
				UpdateMageAngelPhase(ERGX_MageAngelPhase::Phase2);
			}
		}
		break;
	case ERGX_MageAngelPhase::Phase2:
		{
			if (proportion < Phase3Threshold)
			{
				UpdateMageAngelPhase(ERGX_MageAngelPhase::Phase3);
			}
		}
		break;
	}
}

void ARGX_MageAngelController::BeginPlay()
{
	Super::BeginPlay();
}

void ARGX_MageAngelController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	StartLogic();

	UpdateMageAngelPhase(ERGX_MageAngelPhase::Phase1);
}

void ARGX_MageAngelController::UpdateMageAngelPhase(ERGX_MageAngelPhase::Type Phase)
{
	const FName tagName = "AI.MageAngel.AttackBehavior";
	const FGameplayTag attackTag = FGameplayTag::RequestGameplayTag(tagName);

	switch (Phase)
	{
	case ERGX_MageAngelPhase::Phase1:
		BehaviorTreeComponent->SetDynamicSubtree(attackTag, Phase1Tree);
		break;
	case ERGX_MageAngelPhase::Phase2:
		BehaviorTreeComponent->SetDynamicSubtree(attackTag, Phase2Tree);
		break;
	case ERGX_MageAngelPhase::Phase3:
		BehaviorTreeComponent->SetDynamicSubtree(attackTag, Phase3Tree);
		break;
	}
	
	MageAngelPhase = Phase;
}
