// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_DistanceAngelController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "RegicideX\Actors\Enemies\RGX_DistanceAngel.h"
#include "NavigationSystem.h"

ARGX_DistanceAngelController::ARGX_DistanceAngelController()
{
	
}

FVector ARGX_DistanceAngelController::GenerateRandomLocationAroundPoint(FVector Location, float Radius) const
{
	return UNavigationSystemV1::GetRandomReachablePointInRadius(GetWorld(), Location, Radius);
}

void ARGX_DistanceAngelController::SetRandomLocationAroundTarget(float Distance)
{
	ARGX_DistanceAngel* DistAngel = Cast<ARGX_DistanceAngel>(GetPawn());
	const FVector OriginalLocation = DistAngel->GetActorLocation();
	FVector LocationAroundPlayer = GenerateRandomLocationAroundPoint(DistAngel->TargetActor->GetTargetLocation(), Distance);
	LocationAroundPlayer.Z = OriginalLocation.Z;
	UBlackboardComponent* BB = GetBlackboardComponent();
	BB->SetValueAsVector(TEXT("TeleportLocation"), LocationAroundPlayer);
}
