// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_BTTaskLaserBeamAttack.h"

#include "RegicideX\Actors\Weapons\RGX_LaserBeamWeapon.h"

#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"

EBTNodeResult::Type URGX_BTTaskLaserBeamAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	OwnerActor = AIController->GetPawn();

	FActorSpawnParameters SpawnParams; 
	SpawnParams.Owner = OwnerActor;
	SpawnParams.Instigator = OwnerActor;

	LaserWeapon = GetWorld()->SpawnActor<ARGX_LaserBeamWeapon>(LaserBeamClass, SpawnParams);
	LaserWeapon->SetSourcePoint(OwnerActor->GetActorLocation());

	bNotifyTick = true;
	return EBTNodeResult::InProgress;
}

void URGX_BTTaskLaserBeamAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	TaskTime += DeltaSeconds;
	if (TaskTime >= MaxTime)
	{
		TaskTime = 0.0f;
		LaserWeapon->Destroy();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	const FVector ActorLocation = OwnerActor->GetActorLocation();
	const FVector EndPointLocation = LaserWeapon->EndPointMesh->K2_GetComponentLocation();

	const FRotator RotOffset = UKismetMathLibrary::FindLookAtRotation(ActorLocation, EndPointLocation);
	OwnerActor->SetActorRotation(RotOffset);

	LaserWeapon->MoveAndDrawRay(DeltaSeconds);

	FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
}
