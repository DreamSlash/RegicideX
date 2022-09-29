
#include "RGX_PeasantController.h"

#include "RegicideX/Actors/Enemies/RGX_Peasant.h"

ARGX_PeasantController::ARGX_PeasantController(const FObjectInitializer& ObjectInitializer)
	: ARGX_EnemyBaseController(ObjectInitializer)
{

}

void ARGX_PeasantController::OnPossess(APawn* pawn)
{
	if (!pawn)
	{
		UE_LOG(LogTemp, Error, TEXT("No pawn to be possessed!\n"));
		return;
	}

	Super::OnPossess(pawn);

	Peasant = Cast<ARGX_Peasant>(pawn);

	//Hack temporal, se lanza el bt desde blueprints pq antes se hace un playmontage en el onpossess
	
	//if (Peasant)
	//{
	//	if (Peasant->BTree)
	//	{
	//		if (BBComponent->InitializeBlackboard(*Peasant->BTree->BlackboardAsset))
	//		{
	//			TargetActorID = BBComponent->GetKeyID("TargetActor");
	//			DistanceToPlayerID = BBComponent->GetKeyID("DistanceToPlayer");

	//			// [SM] hack pero esto hay que arreglarlo
	//			BBComponent->SetValueAsObject("SelfActor", GetPawn());

	//			// Execute behavior tree after initialization.
	//			BTComponent->StartTree(*Peasant->BTree, EBTExecutionMode::Looped);
	//		}		
	//	}
	//}
}

void ARGX_PeasantController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*Peasant = Cast<ARGX_Peasant>(GetPawn());*/
}
