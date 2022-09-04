#include "RGX_BTTask_EnemyBaseTeleport.h"

#include "AIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"

EBTNodeResult::Type URGX_BTTask_EnemyBaseTeleport::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* aiController = OwnerComp.GetAIOwner();
	ARGX_EnemyBase* enemy = Cast<ARGX_EnemyBase>(aiController->GetPawn());
	const AActor* target = enemy->TargetActor;

	const FVector teleportLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BlackboardKey.SelectedKeyName);
	const FVector targetLocation = target->GetActorLocation();
	const FRotator rotator = UKismetMathLibrary::FindLookAtRotation(teleportLocation, targetLocation);

	enemy->SetActorLocationAndRotation(teleportLocation, rotator.Quaternion());

	return EBTNodeResult::Succeeded;
}