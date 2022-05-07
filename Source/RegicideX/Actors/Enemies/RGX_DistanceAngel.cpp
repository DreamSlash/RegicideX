// Fill out your copyright notice in the Description page of Project Settings.

#include "RGX_DistanceAngel.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Components/SphereComponent.h"
#include "Components/MCV_AbilitySystemComponent.h"
#include "NavigationSystem.h"
#include "Kismet/KismetSystemLibrary.h"

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
	HeightPos = GetActorLocation().Z;
	RingOriginalRotatingSpeed = RingRotatingSpeed;
}

void ARGX_DistanceAngel::MoveToTarget(float DeltaTime, FVector TargetPos)
{
	Super::MoveToTarget(DeltaTime, TargetPos);

	FVector Location = this->GetActorLocation();
	Location.Z = HeightPos;
	this->SetActorLocation(Location);
}

void ARGX_DistanceAngel::RotateRings(float DeltaTime) 
{
	float speed = RingRotatingSpeed * DeltaTime;
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

	FHitResult Result;

	FVector NewLocation = GetActorLocation();

	if(GetWorld()->LineTraceSingleByChannel(Result, GetActorLocation() + DownVector*(HeightPos/2.0f), GetActorLocation() + DownVector * HeightPos * 2.0f, ECollisionChannel::ECC_WorldStatic))
	{
		FVector ImpactPoint = Result.ImpactPoint;
		NewLocation.Z = ImpactPoint.Z + ActorMidHeight;

		/*if (Result.Actor != nullptr) {
			AActor* actor = Cast<AActor>(Result.Actor);
			UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s"), *actor->GetName());
		}
		
		UKismetSystemLibrary::DrawDebugLine(
			GetWorld(),
			GetActorLocation(),
			ImpactPoint,
			FColor(255, 0, 0),
			22.0f,
			5.0f
			);*/
	}

	SetActorLocation(NewLocation);

}

void ARGX_DistanceAngel::TPToOriginalHeight()
{
	FVector NewLocation = GetActorLocation();
	NewLocation.Z = HeightPos;
	SetActorLocation(NewLocation);
}

void ARGX_DistanceAngel::ShootSimpleBullets()
{
	FVector MyFront = this->GetActorForwardVector();
	FRotator MyRotation = this->GetActorRotation();
	for(FVector Position : BombingPoints)
	{
		FRotator BulletRotation = MyRotation; 
		FVector BulletNormRotationVector = BulletRotation.Vector(); BulletNormRotationVector.Normalize();

		float RotYawOffset = FMath::RandRange(-180.0f, 180.0f);
		BulletRotation.Yaw += RotYawOffset;

		float RotPitchOffset = FMath::RandRange(-45.0f, 45.0f);
		BulletRotation.Pitch += RotPitchOffset;

		FVector BulletPosition = this->GetActorLocation() + FVector(0.0, 0.0, Position.Z);
		FVector BulletScale(0.1);
		FTransform BulletTransform(BulletRotation, BulletPosition, BulletScale);
		SpawnSimpleBullet(BulletTransform, this);
	}

}

void ARGX_DistanceAngel::TestSpawn()
{
	FVector Location(0.0f, 0.0f, 300.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FVector Scale(1.0f);
	ARGX_DistanceAngel* Actor = GetWorld()->SpawnActor<ARGX_DistanceAngel>(Location, Rotation);
	UE_LOG(LogTemp, Warning, TEXT("ESPAWNEJANT L'ACTOR: %s"), *Actor->GetName());
}

FVector ARGX_DistanceAngel::GenerateRandomLocationAroundPoint(FVector Location)
{
	return UNavigationSystemV1::GetRandomReachablePointInRadius(GetWorld(), Location, AttackRadius);
}


float ARGX_DistanceAngel::GetDistanceToTarget()
{
	return FVector::Distance(this->GetActorLocation(), TargetActor->GetActorLocation());
}

void ARGX_DistanceAngel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateRings(DeltaTime);
}

