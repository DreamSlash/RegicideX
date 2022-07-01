// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/Actors/Projectiles/RGX_SimpleProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "RegicideX/Components/RGX_HitboxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ARGX_SimpleProjectile::ARGX_SimpleProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	HitboxComponent = CreateDefaultSubobject<URGX_HitboxComponent>(TEXT("HitboxComponent"));
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	RootComponent = ProjectileMesh;
	HitboxComponent->SetupAttachment(RootComponent);
}

void ARGX_SimpleProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ARGX_SimpleProjectile::OnHitboxOverlap(AActor* OverlappedActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Simple Projectile Overlap\n"));
}
