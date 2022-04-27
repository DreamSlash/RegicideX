// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_PeasantIdle.h"
#include "AIController.h"
#include "RegicideX/Actors/Enemies/RGX_Peasant.h"

EBTNodeResult::Type UBTTask_PeasantIdle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* const PeasantController = OwnerComp.GetAIOwner();
    
    if (PeasantController->GetPawn())
    {
        ARGX_Peasant* const Peasant = Cast<ARGX_Peasant>(PeasantController->GetPawn());
        Peasant->Idle();
    }
    return EBTNodeResult::Succeeded;
}
