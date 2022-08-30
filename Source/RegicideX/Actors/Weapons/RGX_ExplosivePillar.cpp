// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_ExplosivePillar.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"
#include "GameplayTags.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
ARGX_ExplosivePillar::ARGX_ExplosivePillar()
	: ARGX_EffectApplierActor()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	PillarCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("PillarCollider"));
	PillarCollider->SetupAttachment(RootComponent);

	PillarCollider->OnComponentBeginOverlap.AddUniqueDynamic(this, &ARGX_ExplosivePillar::Explode);
}

// Called when the game starts or when spawned
void ARGX_ExplosivePillar::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeTime);
}

void ARGX_ExplosivePillar::Explode(UPrimitiveComponent* OverlappedComponent
	, AActor* OtherActor
	, UPrimitiveComponent* OtherComp
	, int32 OtherBodyIndex
	, bool bFromSweep
	, const FHitResult& SweepResult)
{
	if (OnPlayerOverlaps(OtherActor))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionVFX, GetActorLocation(), GetActorRotation(), FVector(3.0f, 3.0f, 1.0f));
		Destroy();
	}
}
