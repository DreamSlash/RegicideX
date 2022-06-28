
#include "RGX_BTTask_Teleport.h"

#include "AIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "../../Actors/Enemies/RGX_DistanceAngel.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"

EBTNodeResult::Type URGX_BTTask_Teleport::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	FName KeyName = BlackboardKey.SelectedKeyName;

	const AAIController* AIController = OwnerComp.GetAIOwner();
	FVector Location = AIController->GetBlackboardComponent()->GetValueAsVector(KeyName);

	ARGX_DistanceAngel* ControlledPawn = Cast<ARGX_DistanceAngel>(AIController->GetPawn());

	FVector EndDrawLocation = Location; EndDrawLocation.Z = ControlledPawn->GetEyeWorldLocation().Z;

	UKismetSystemLibrary::DrawDebugLine(GetWorld(),ControlledPawn->GetEyeWorldLocation(),EndDrawLocation,FColor(255, 0, 100),44.0,5.0f);

	const FVector CapsuleColliderLocation = ControlledPawn->GetActorLocation();

	Location.Z = CapsuleColliderLocation.Z;
	ControlledPawn->SetActorLocation(Location);

	const FVector RingsColliderLocation = ControlledPawn->GetEyeWorldLocation();
	Location.Z = RingsColliderLocation.Z;

	ControlledPawn->SphereCollider->SetWorldLocation(Location);

	return EBTNodeResult::Succeeded;
}
