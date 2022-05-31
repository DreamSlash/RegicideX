// Fill out your copyright notice in the Description page of Project Settings.

#include "RGX_BTTask_MeleeAngelCharge.h"

#include "RegicideX/Actors/Enemies/RGX_MeleeAngel.h"

#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

EBTNodeResult::Type URGX_BTTask_MeleeAngelCharge::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* ControlledPawn = AIController->GetPawn();

	MeleeAngelPawn = Cast<ARGX_MeleeAngel>(ControlledPawn);

	MeleeAngelPawn->bFlying = true;

	MoveSpeed = MeleeAngelPawn->MoveSpeed;
	GoalLocation = MeleeAngelPawn->TargetActor->GetActorLocation();

	const FRotator RotOffset = UKismetMathLibrary::FindLookAtRotation(MeleeAngelPawn->GetActorLocation(), GoalLocation);
	MeleeAngelPawn->SetActorRotation(RotOffset);

	const FVector RaySrc = MeleeAngelPawn->GetActorLocation() + MeleeAngelPawn->GetActorForwardVector() * 50.0f;
	const FVector RayEnd = MeleeAngelPawn->GetActorLocation() + MeleeAngelPawn->GetActorForwardVector() * 10000.0f;
	FHitResult RayTraceResult;

	GoalLocation = MeleeAngelPawn->GetActorLocation() + MeleeAngelPawn->GetActorForwardVector() * 10000.0f;

	if (GetWorld()->LineTraceSingleByChannel(RayTraceResult, RaySrc, RayEnd, ECollisionChannel::ECC_WorldStatic))
	{
		if (RayTraceResult.ImpactPoint != FVector(0.0, 0.0, 0.0))
		{
			GoalLocation = RayTraceResult.ImpactPoint;
		}
	}

	UKismetSystemLibrary::DrawDebugLine(
		GetWorld(),
		RaySrc,
		RayEnd,
		FColor(255, 0, 0),
		5.0,
		5.0f
	);

	UKismetSystemLibrary::DrawDebugPoint(GetWorld(), GoalLocation, 100.0f, FColor(255, 255, 255), 5.0f);

	bNotifyTick = true;

	return EBTNodeResult::InProgress;
}

void URGX_BTTask_MeleeAngelCharge::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	TaskTime += DeltaSeconds;
	float dist = FVector::Distance(MeleeAngelPawn->GetActorLocation(), GoalLocation);
	UE_LOG(LogTemp, Warning, TEXT("Dist to goal pos: %f"), dist);
	if (TaskTime >= MaxTime || MeleeAngelPawn->bCharged || dist <= 30.0f)
	{
		TaskTime = 0.0f;
		MeleeAngelPawn->bSimGravityDisabled = true;
		MeleeAngelPawn->bFlying = false;
		MeleeAngelPawn->bCharged = false;
		MeleeAngelPawn->MoveSpeed = MoveSpeed;
		FRotator Rotation = MeleeAngelPawn->GetActorRotation();
		Rotation.Pitch = 0.0;
		MeleeAngelPawn->SetActorRotation(Rotation);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	MeleeAngelPawn->MoveSpeed += TaskTime * AccelerationMultiplier;
	MeleeAngelPawn->MoveToTarget(DeltaSeconds, FVector());
}










