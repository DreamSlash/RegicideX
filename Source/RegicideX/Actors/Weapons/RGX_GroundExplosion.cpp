// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_GroundExplosion.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "GameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
ARGX_GroundExplosion::ARGX_GroundExplosion()
	: ARGX_EffectApplierActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	ExplosionCollider = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollider"));
	ExplosionCollider->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ARGX_GroundExplosion::BeginPlay()
{
	Super::BeginPlay();

	FHitResult result;
	FVector location = GetActorLocation();
	if (GetWorld()->LineTraceSingleByChannel(result, location, location * FVector(1, 1, -1), ECollisionChannel::ECC_WorldStatic))
	{
		location.Z = result.ImpactPoint.Z;
	}
	UNiagaraComponent* tell = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TellVFX, location, GetActorRotation(), FVector(3.0f, 3.0f, 1.0f), true, false);
	tell->SetFloatParameter(DurationParameter, ExplosionTime);
	tell->ActivateSystem();

	GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, [this]() { Explode(); }, ExplosionTime, false);
}

void ARGX_GroundExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARGX_GroundExplosion::Explode()
{
	TArray<AActor*> OverlappedActors;
	ExplosionCollider->GetOverlappingActors(OverlappedActors, TargetActor);

	for (AActor* Actor : OverlappedActors)
	{
		if (OnPlayerOverlaps(Actor))
		{

		}
	}

	// Fix Z to hit the ground
	FHitResult result;
	FVector location = GetActorLocation();
	//DrawDebugLine(GetWorld(), location, location * FVector(1, 1, -1), FColor(255, 0, 0), true, 5.0f, 0, 5.0f);
	if (GetWorld()->LineTraceSingleByChannel(result, location, location * FVector(1, 1, -1), ECollisionChannel::ECC_WorldStatic))
	{
		location.Z = result.ImpactPoint.Z;
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionVFX, location, GetActorRotation(), FVector(3.0f, 3.0f, 1.0f));

	Destroy();
}
