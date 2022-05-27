// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_BTTask_PeasantDestroy.h"
#include "AIController.h"
#include "RegicideX/Actors/Enemies/RGX_Peasant.h"
#include "RegicideX/Actors/Enemies/RGX_GroupManager.h"

EBTNodeResult::Type URGX_BTTask_PeasantDestroy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ARGX_Peasant* Peasant = OwnerComp.GetOwner<ARGX_Peasant>();
    AAIController* AiController = OwnerComp.GetAIOwner();
    APawn* Pawn = AiController->GetPawn();

    UE_LOG(LogTemp, Display, TEXT("Execute task Peasant Destroy"));

    if (Peasant != 0)
    {
        Peasant->manager->RemovePeasant(Peasant);
        GetWorld()->GetTimerManager().SetTimer(Peasant->CorpseTimerHandle, Peasant, &ARGX_Peasant::DestroyPeasant, 2.0f, false);

        AAIController* Controller = Cast<AAIController>(Peasant->GetController());
        if (Controller)
        {
            Controller->UnPossess();
            Controller->Destroy();
        }
        return EBTNodeResult::Succeeded;
    }
    return EBTNodeResult::Failed;
}
