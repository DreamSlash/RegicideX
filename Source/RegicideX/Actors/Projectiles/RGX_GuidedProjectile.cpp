// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/Actors/Projectiles/RGX_GuidedProjectile.h"
#include "AbilitySystemComponent.h"
#include "RegicideX/Actors/RGX_CharacterBase.h"
#include "RegicideX/GAS/RGX_GameplayEffectContext.h"
#include "Components/StaticMeshComponent.h"
#include "RegicideX/Components/RGX_HitboxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

ARGX_GuidedProjectile::ARGX_GuidedProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	HitboxComponent = CreateDefaultSubobject<URGX_HitboxComponent>(TEXT("HitboxComponent"));

	RootComponent = ProjectileMesh;
	HitboxComponent->SetupAttachment(RootComponent);

	TargetActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

void ARGX_GuidedProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckDistance();
	RotateToTarget(DeltaTime);
	Move(DeltaTime);
}

void ARGX_GuidedProjectile::RotateToTarget(float DeltaTime)
{
	if (TargetActor && bStopFollowing == false)
	{
		const FVector MyLocation = this->GetActorLocation();
		const FVector TargetLocation = TargetActor->GetActorLocation();
		const FRotator RotOffset = UKismetMathLibrary::FindLookAtRotation(MyLocation, TargetLocation);
		this->SetActorRotation(RotOffset);
	}
}

void ARGX_GuidedProjectile::CheckDistance()
{
	if (TargetActor)
	{
		const FVector MyLocation = this->GetActorLocation();
		const FVector TargetLocation = TargetActor->GetActorLocation();

		const  float Dist = FVector::Distance(MyLocation, TargetLocation);
		if (Dist <= StopFollowingDistance)
		{
			bStopFollowing = true;
		}
	}
}

void ARGX_GuidedProjectile::Move(float DeltaTime)
{
	const FVector MyFront = this->GetActorForwardVector();
	const FVector CurrentLocation = this->GetActorLocation();
	const FVector NewLocation = CurrentLocation + MyFront * BaseSpeed * DeltaTime;

	SetActorLocation(NewLocation);
}
