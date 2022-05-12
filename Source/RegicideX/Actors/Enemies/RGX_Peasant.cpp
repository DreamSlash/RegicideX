// Fill out your copyright notice in the Description page of Project Settings.

#include "RGX_Peasant.h"
#include "Components/MCV_AbilitySystemComponent.h"
#include "EngineUtils.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RGX_GroupManager.h"
#include "Kismet/GameplayStatics.h"

ARGX_Peasant::ARGX_Peasant()
{
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

	AddGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("Status.Alive")));
}

void ARGX_Peasant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ToBeDestroyed)
		manager->RemovePeasant(this);

	GetCharacterMovement()->MaxWalkSpeed = bInCombat ? 400.0f : 100.0f;

	if (bInCombat && !manager->AttackingPeasant)
	{
		manager->AttackingPeasant = this;
		bAttacking = true;
	}
}

void ARGX_Peasant::Idle()
{
	// TODO Rotate to target
	TextStatusString = "Idle";
}

void ARGX_Peasant::ResetAttacking()
{
	if (manager)
	{
		bAttacking = false;
		manager->AttackingPeasant = nullptr;
	}
}

float ARGX_Peasant::GetDistanceToTarget()
{
	if (!TargetActor)
		return 0.0f;
	return FVector::Distance(TargetActor->GetActorLocation(), GetActorLocation());
}