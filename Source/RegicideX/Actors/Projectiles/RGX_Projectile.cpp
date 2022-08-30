// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/Actors/Projectiles/RGX_Projectile.h"
#include "Components/StaticMeshComponent.h"

ARGX_Projectile::ARGX_Projectile()
{

}

// Called when the game starts or when spawned
void ARGX_Projectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARGX_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARGX_Projectile::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	ProjectileTeam = TeamID;
}

FGenericTeamId ARGX_Projectile::GetGenericTeamId() const
{
	return ProjectileTeam;
}


