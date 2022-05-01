// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_DistanceAngelController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "RegicideX\Actors\Enemies\RGX_DistanceAngel.h"

ARGX_DistanceAngelController::ARGX_DistanceAngelController()
{
	
}


void ARGX_DistanceAngelController::SetRandomLocationAroundTarget()
{
	ARGX_DistanceAngel* DistAngel = Cast<ARGX_DistanceAngel>(GetPawn());
	FVector OriginalLocation = DistAngel->GetActorLocation();
	FVector LocationAroundPlayer = DistAngel->GenerateRandomLocationAroundPoint(DistAngel->TargetActor->GetTargetLocation());
	LocationAroundPlayer.Z = OriginalLocation.Z;
	UBlackboardComponent* BB = GetBlackboardComponent();
	BB->SetValueAsVector(TEXT("TeleportLocation"), LocationAroundPlayer);
}
