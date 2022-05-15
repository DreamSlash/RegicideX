
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

	if (Peasant)
	{
		if (Peasant->BTree)
		{
			if (BBComponent->InitializeBlackboard(*Peasant->BTree->BlackboardAsset))
			{
				TargetActorID = BBComponent->GetKeyID("TargetActor");
				DistanceToPlayerID = BBComponent->GetKeyID("DistanceToPlayer");
				OnAirID = BBComponent->GetKeyID("bOnAir");
				WasHitID = BBComponent->GetKeyID("bWasHit");
				InCombatID = BBComponent->GetKeyID("bInCombat");
				AttackingID = BBComponent->GetKeyID("bAttacking");
				IdleActionID = BBComponent->GetKeyID("IdleAction");
				IdlePositionID = BBComponent->GetKeyID("IdlePosition");

				// Execute behavior tree after initialization.
				BTComponent->StartTree(*Peasant->BTree, EBTExecutionMode::Looped);
			}		
		}
	}
}

void ARGX_PeasantController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ARGX_Peasant* Peasant = Cast<ARGX_Peasant>(GetPawn());

	if (Peasant->TargetActor && bIsInFocus == false)
	{
		SetFocus(Peasant->TargetActor);
		bIsInFocus = true;
	}

	if (Peasant)
	{
		// Update values for the BB of the BT.
		if(Peasant->TargetActor)
			BBComponent->SetValueAsObject("TargetActor", Peasant->TargetActor);

		// Do not do it every tick
		BBComponent->SetValueAsFloat("DistanceToPlayer", Peasant->GetDistanceToTarget());
		BBComponent->SetValueAsBool("bWasHit", Peasant->bWasHit);
		BBComponent->SetValueAsBool("bOnAir", Peasant->bOnAir);
		BBComponent->SetValueAsBool("bInCombat", Peasant->bInCombat);
		BBComponent->SetValueAsBool("bAttacking", Peasant->bAttacking);
		BBComponent->SetValueAsVector("IdlePosition", Peasant->IdlePosition);
		BBComponent->SetValueAsInt("IdleAction", Peasant->IdleAction);
	}
}
