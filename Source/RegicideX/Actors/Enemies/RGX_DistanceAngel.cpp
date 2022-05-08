// Fill out your copyright notice in the Description page of Project Settings.

#include "RGX_DistanceAngel.h"

#include "Components/MCV_AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "NavigationSystem.h"


ARGX_DistanceAngel::ARGX_DistanceAngel()
	: ARGX_EnemyBase()
{
	Ring_1_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ring1"));
	Ring_2_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ring2"));
	Ring_3_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ring3"));

	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));

	//FAttachmentTransformRules attachment_rules(EAttachmentRule::KeepRelative, false);
	SphereCollider->SetupAttachment(RootComponent);
	Ring_1_Mesh->SetRelativeLocation(FVector(0.0));
	Ring_2_Mesh->SetRelativeLocation(FVector(0.0));
	Ring_3_Mesh->SetRelativeLocation(FVector(0.0));
	Ring_1_Mesh->SetupAttachment(SphereCollider);
	Ring_2_Mesh->SetupAttachment(Ring_1_Mesh);
	Ring_3_Mesh->SetupAttachment(Ring_1_Mesh);
}

void ARGX_DistanceAngel::BeginPlay()
{
	Super::BeginPlay();
	SetLocationHeight(HeightPos);
	RingOriginalRotatingSpeed = RingRotatingSpeed;
}

void ARGX_DistanceAngel::MoveToTarget(float DeltaTime, FVector TargetPos)
{
	Super::MoveToTarget(DeltaTime, TargetPos);
	SetLocationHeight(HeightPos);
}

void ARGX_DistanceAngel::RotateRings(float DeltaTime) 
{
	const float speed = RingRotatingSpeed * DeltaTime;
	Ring_2_Mesh->AddLocalRotation(FRotator(-speed, 0.0, speed));
	Ring_3_Mesh->AddLocalRotation(FRotator(0.0, speed, speed));
}

void ARGX_DistanceAngel::RotateMe(float DeltaTime, float Speed)
{
	FRotator NewRotation(0.0);
	NewRotation.Yaw += FullBodyRotatingSpeed * DeltaTime * Speed;
	SetActorRotation(NewRotation);
}

void ARGX_DistanceAngel::TPToFloor()
{
	const FVector DownVector = -GetActorUpVector();
	float NewHeight = HeightPos;

	FHitResult Result;

	const float DownRaySrcOffset = HeightPos / 2.0f;
	const float DownRayEndOffset = HeightPos * 2.0f;

	const FVector ActorLocation = GetActorLocation();

	if(GetWorld()->LineTraceSingleByChannel(Result, ActorLocation + DownVector * DownRaySrcOffset, ActorLocation + DownVector * DownRayEndOffset, ECollisionChannel::ECC_WorldStatic))
	{
		NewHeight = Result.ImpactPoint.Z + ActorMidHeight;
	}

	SetLocationHeight(NewHeight);

}

void ARGX_DistanceAngel::TPToOriginalHeight()
{
	SetLocationHeight(HeightPos);
}

FVector ARGX_DistanceAngel::GenerateRandomLocationAroundPoint(FVector Location) const
{
	return UNavigationSystemV1::GetRandomReachablePointInRadius(GetWorld(), Location, AttackRadius);
}

float ARGX_DistanceAngel::GetDistanceToTarget() const
{
	return FVector::Distance(this->GetActorLocation(), TargetActor->GetActorLocation());
}

void ARGX_DistanceAngel::SetLocationHeight(float Height) {
	FVector NewLocation = GetActorLocation();
	NewLocation.Z = Height;
	SetActorLocation(NewLocation);
}

void ARGX_DistanceAngel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotateRings(DeltaTime);
}

