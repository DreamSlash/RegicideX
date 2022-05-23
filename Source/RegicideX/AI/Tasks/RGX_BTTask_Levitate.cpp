// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_BTTask_Levitate.h"

#include "RegicideX/Actors/Enemies/RGX_MeleeAngel.h"

#include "AIController.h"
#include "Kismet/KismetSystemLibrary.h"

EBTNodeResult::Type URGX_BTTask_Levitate::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	APawn* ControlledPawn = AIController->GetPawn();
	MeleeAngelPawn = Cast<ARGX_MeleeAngel>(ControlledPawn);

	const FVector RaySrc = MeleeAngelPawn->GetActorLocation() + MeleeAngelPawn->GetActorUpVector() * 160.0f;
	const FVector RayEnd = MeleeAngelPawn->GetActorLocation() + MeleeAngelPawn->GetActorUpVector() * MaxLHeight;
	FHitResult RayTraceResult;

	GoalLocation = MeleeAngelPawn->GetActorLocation() + FVector(0.0, 0.0, MaxLHeight);

	if (GetWorld()->LineTraceSingleByChannel(RayTraceResult, RaySrc, RayEnd, ECollisionChannel::ECC_WorldStatic))
	{

		if(RayTraceResult.ImpactPoint != FVector(0.0,0.0,0.0))
		{
			GoalLocation = RayTraceResult.ImpactPoint;
		}

	}

	/*UKismetSystemLibrary::DrawDebugLine(
		GetWorld(),
		RaySrc,
		RayEnd,
		FColor(255, 0, 0),
		10.0,
		5.0f
	);*/

	MeleeAngelPawn->bLevitating = true;

	bNotifyTick = true;
	return EBTNodeResult::InProgress;
}

void URGX_BTTask_Levitate::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	TaskTime += DeltaSeconds;

	if (TaskTime >= MaxTime){
		MeleeAngelPawn->bLevitating = false;
		TaskTime = 0.0f;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	float DistToGoal = FVector::Dist(MeleeAngelPawn->GetActorLocation(), GoalLocation);
	UE_LOG(LogTemp, Warning, TEXT("DistToGoal: %f"), DistToGoal);
	if(DistToGoal <= 2.0 ) return;
	const FVector NewLocation = FMath::Lerp(MeleeAngelPawn->GetActorLocation(), GoalLocation, DeltaSeconds * LevitateSpeed);
	MeleeAngelPawn->SetActorLocation(NewLocation);
}
