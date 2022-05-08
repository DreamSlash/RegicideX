// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_BTServiceAtivateAbility.h"

#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"




URGX_BTServiceAtivateAbility::URGX_BTServiceAtivateAbility() 
{

}


void URGX_BTServiceAtivateAbility::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* Pawn = AIController->GetPawn();

	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn);
	ASC->TryActivateAbilityByClass(AbilityToActivte);
}
