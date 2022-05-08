
#include "RGX_BTTask_Teleport.h"

#include "AIController.h"

#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type URGX_BTTask_Teleport::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FName KeyName = BlackboardKey.SelectedKeyName;
	const AAIController* AIController = OwnerComp.GetAIOwner();
	FVector Location = AIController->GetBlackboardComponent()->GetValueAsVector(KeyName);

	APawn* ControlledPawn = AIController->GetPawn();

	ControlledPawn->SetActorLocation(Location);

	return EBTNodeResult::Succeeded;
}

