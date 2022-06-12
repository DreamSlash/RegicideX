// Fill out your copyright notice in the Description page of Project Settings.

#include "RGX_Peasant.h"
#include "Components/MCV_AbilitySystemComponent.h"
#include "EngineUtils.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ARGX_Peasant::ARGX_Peasant()
{
}

void ARGX_Peasant::BeginPlay()
{
	
	Super::BeginPlay();

	AddGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("Status.Alive")));
}

void ARGX_Peasant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	ToBeDestroyed = ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.Dead")));

	// TODO Managed by Manager
	GetCharacterMovement()->MaxWalkSpeed = bInCombat ? MaxSpeed : WanderSpeed;
}

void ARGX_Peasant::HandleDeath()
{
	UE_LOG(LogTemp, Display, TEXT("Handling Peasant death ..."));
	Super::HandleDeath();
}

void ARGX_Peasant::DestroyPeasant()
{
	// Handle particles and stuff before dying ...
	this->Destroy();
}

// TODO Make it in EnemyBase --> Make it 2D??
float ARGX_Peasant::GetDistanceToTarget() const
{
	if (!TargetActor)
		return 0.0f;
	return FVector::Distance(TargetActor->GetActorLocation(), GetActorLocation());
}