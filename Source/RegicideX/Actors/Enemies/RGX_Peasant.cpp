// Fill out your copyright notice in the Description page of Project Settings.

#include "EngineUtils.h"
#include "RGX_Peasant.h"
#include "RGX_GroupManager.h"
#include "Kismet/GameplayStatics.h"

ARGX_Peasant::ARGX_Peasant()
{
}

void ARGX_Peasant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARGX_Peasant::Idle()
{
	RotateToTarget();
	TextStatusString = "Idle";
}

void ARGX_Peasant::Attack()
{
	RotateToTarget();
	TextStatusString = "Attacking";
	PlayAnimMontage(PunchMontage);
}

void ARGX_Peasant::BeginPlay()
{
	Super::BeginPlay();
	TargetActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	for (TActorIterator<ARGX_GroupManager> MngItr(GetWorld()); MngItr; ++MngItr)
	{
		manager = *MngItr;
		break;
	}
	if(manager)
		manager->AddPeasant(this);
}

void ARGX_Peasant::RotateToTarget()
{
	FVector Position = GetActorLocation();
	FVector TargetPosition = TargetActor->GetActorLocation();
	FVector DirectionToTarget = TargetPosition - Position;
	DirectionToTarget.Normalize();

	FRotator Rotation = DirectionToTarget.Rotation();
	SetActorRotation(Rotation);
}
