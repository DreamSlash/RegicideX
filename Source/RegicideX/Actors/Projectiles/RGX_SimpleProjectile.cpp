// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/Actors/Projectiles/RGX_SimpleProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "RegicideX/Components/RGX_HitboxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "RegicideX/Actors/RGX_CharacterBase.h"
#include "RegicideX/GAS/RGX_GameplayEffectContext.h"

ARGX_SimpleProjectile::ARGX_SimpleProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	HitboxComponent = CreateDefaultSubobject<URGX_HitboxComponent>(TEXT("HitboxComponent"));
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	RootComponent = ProjectileMesh;
	HitboxComponent->SetupAttachment(RootComponent);
}