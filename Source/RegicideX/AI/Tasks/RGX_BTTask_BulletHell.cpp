// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_BTTask_BulletHell.h"

#include "RegicideX/Actors/Enemies/RGX_DistanceAngel.h"

#include "AIController.h"
#include "Components/SphereComponent.h"

EBTNodeResult::Type URGX_BTTask_BulletHell::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* ControlledPawn = AIController->GetPawn();

	DistAngelPawn = Cast<ARGX_DistanceAngel>(ControlledPawn);

	OriginalBulletHellSphereScale = 10.0;
	DistAngelPawn->BulletHellSphereCollider->SetRelativeScale3D(FVector(OriginalBulletHellSphereScale));
	DistAngelPawn->BulletHellSphere->SetHiddenInGame(false);
	//DistAngelPawn->BulletHellOutSphere->SetHiddenInGame(false);
	DistAngelPawn->BulletHellOutSphere->SetRelativeScale3D(FVector(MaxSize));
	bNotifyTick = true;
	return EBTNodeResult::InProgress;

}

void URGX_BTTask_BulletHell::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	TaskTime += DeltaSeconds;
	if(TaskTime >= MaxTime)
	{
		TaskTime = 0.0f;
		DistAngelPawn->BulletHellSphere->SetRelativeScale3D(FVector(OriginalBulletHellSphereScale));
		DistAngelPawn->BulletHellSphere->SetHiddenInGame(true);
		//DistAngelPawn->BulletHellOutSphere->SetHiddenInGame(true);
		DistAngelPawn->BulletHellSphereCollider->SetRelativeScale3D(FVector(6.5));
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	DistAngelPawn->RotateMe(DeltaSeconds, 1.0 + TaskTime * RotationSpeedMultiplier);

	FVector CurrentScale = DistAngelPawn->BulletHellSphere->GetRelativeScale3D();
	if (CurrentScale.Z < MaxSize) 
	{
		FVector NewScale = DistAngelPawn->BulletHellSphere->GetRelativeScale3D() + FVector(TaskTime) * 0.4;
		DistAngelPawn->BulletHellSphere->SetRelativeScale3D(NewScale);
		FVector NewScale2 = DistAngelPawn->BulletHellSphereCollider->GetRelativeScale3D() + FVector(TaskTime) * 0.4;
		DistAngelPawn->BulletHellSphereCollider->SetRelativeScale3D(NewScale2);
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);

}






