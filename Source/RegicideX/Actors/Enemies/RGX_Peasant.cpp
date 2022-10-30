// Fill out your copyright notice in the Description page of Project Settings.

#include "RGX_Peasant.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Components/MCV_AbilitySystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"

#include "RegicideX/Components/RGX_MovementAssistComponent.h"

ARGX_Peasant::ARGX_Peasant()
{
	MovementAssistComponent = CreateDefaultSubobject<URGX_MovementAssistComponent>(TEXT("MovementAssistComponent"));
	TellVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TellNiagaraComponent"));
	TellVFX->SetupAttachment(GetMesh());

	GetMesh()->SetHiddenInGame(true, true);
	bImmune = true;
}

void ARGX_Peasant::BeginPlay()
{
	
	Super::BeginPlay();

	AddGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("Status.Alive")));
	SetActorEnableCollision(true);
	bDefaultFocusPlayer = true;
}

void ARGX_Peasant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARGX_Peasant::ActivateTellVFX()
{
	TellVFX->Activate(true);

	GetWorld()->GetTimerManager().SetTimer(TellVFXTimerHandle, [this]() { TellVFX->Deactivate(); }, TellVFXTime, false);
}

void ARGX_Peasant::Activate()
{
	Super::Activate();

	//SetActorEnableCollision(true);
	//GetMesh()->SetScalarParameterValueOnMaterials(FName("Amount (S)"), 0.0f); // Recover from evaporation effect when dying
	//GetMesh()->bPauseAnims = false; // Recover from pausing anims in GA_PeasantDeath

	//AAIController* PeasantController = Cast<AAIController>(GetController());
	//if (PeasantController)
	//{
	//	PeasantController->GetBrainComponent()->StartLogic();
	//}
}

void ARGX_Peasant::Deactivate()
{
	Super::Deactivate();
}

void ARGX_Peasant::HandleDeath()
{
	UE_LOG(LogTemp, Display, TEXT("Handling Peasant death ..."));
	Super::HandleDeath();
}

// TODO Make it in EnemyBase --> Make it 2D??
float ARGX_Peasant::GetDistanceToTarget() const
{
	if (!TargetActor)
		return 0.0f;
	return FVector::Distance(TargetActor->GetActorLocation(), GetActorLocation());
}