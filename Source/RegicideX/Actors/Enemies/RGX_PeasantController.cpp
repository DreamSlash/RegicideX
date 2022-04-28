// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_PeasantController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "RGX_Peasant.h"

ARGX_PeasantController::ARGX_PeasantController()
{
	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BBPeasant"));
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BTPeasant"));
}

void ARGX_PeasantController::OnPossess(APawn* pawn)
{
	if (!pawn)
	{
		UE_LOG(LogTemp, Error, TEXT("No pawn to be possessed!\n"));
		return;
	}

	Super::OnPossess(pawn);
	ARGX_Peasant* Peasant = Cast<ARGX_Peasant>(pawn);

	if (Peasant && Peasant->BTree)
	{
		if (BBComponent->InitializeBlackboard(*Peasant->BTree->BlackboardAsset))
		{
			TargetActorID = BBComponent->GetKeyID("TargetActor");
			PositionID = BBComponent->GetKeyID("pos");
			OnAirID = BBComponent->GetKeyID("bOnAir");
			WasHitID = BBComponent->GetKeyID("bWasHit");
			InCombatID = BBComponent->GetKeyID("bInCombat");
			IdlePositionID = BBComponent->GetKeyID("IdlePosition");

			// Execute behavior tree after initialization.
			BTComponent->StartTree(*Peasant->BTree, EBTExecutionMode::Looped);
		}		
	}
}

void ARGX_PeasantController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ARGX_Peasant* Peasant = Cast<ARGX_Peasant>(GetPawn());

	if (Peasant)
	{
		// Debug log ...
		if (BTComponent->IsRunning())
			UE_LOG(LogTemp, Display, TEXT("BTComponent is running ...\n"));

		// Update values for the BB of the BT.
		if(Peasant->TargetActor)
			BBComponent->SetValueAsObject("TargetActor", Peasant->TargetActor);

		BBComponent->SetValueAsVector("pos", FVector(0.0f));
		BBComponent->SetValueAsBool("bWasHit", Peasant->bWasHit);
		BBComponent->SetValueAsBool("bOnAir", Peasant->bOnAir);
		BBComponent->SetValueAsBool("bInCombat", Peasant->bInCombat);
		BBComponent->SetValueAsVector("IdlePosition", Peasant->IdlePosition);
	}
}
