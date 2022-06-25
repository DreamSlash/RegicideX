// Fill out your copyright notice in the Description page of Project Settings.

#include "RGX_DistanceAngel.h"

#include "Components/MCV_AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "Math/UnrealMathUtility.h"

#include "Components/WidgetComponent.h"

ARGX_DistanceAngel::ARGX_DistanceAngel() : ARGX_EnemyBase()
{
	Ring_1_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ring1"));
	Ring_2_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ring2"));
	Ring_3_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ring3"));
	BulletHellSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletHellSphere"));

	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));

	FloorReturnPlace = CreateDefaultSubobject<USceneComponent>(TEXT("FloorReturnPlace"));

	//FAttachmentTransformRules attachment_rules(EAttachmentRule::KeepRelative, false);
	SphereCollider->SetupAttachment(RootComponent);
	Ring_1_Mesh->SetRelativeLocation(FVector(0.0));
	Ring_2_Mesh->SetRelativeLocation(FVector(0.0));
	Ring_3_Mesh->SetRelativeLocation(FVector(0.0));
	Ring_1_Mesh->SetupAttachment(SphereCollider);
	Ring_2_Mesh->SetupAttachment(Ring_1_Mesh);
	Ring_3_Mesh->SetupAttachment(Ring_1_Mesh);
	BulletHellSphere->SetupAttachment(SphereCollider);

	FloorReturnPlace->SetRelativeLocation(FVector(0.0));
	FloorReturnPlace->SetupAttachment(RootComponent);

	HealthDisplayWidgetComponent->SetupAttachment(SphereCollider);
	CombatTargetWidgetComponent->SetupAttachment(SphereCollider);

	BulletHellSphere->SetHiddenInGame(true);
}

void ARGX_DistanceAngel::BeginPlay()
{
	Super::BeginPlay();
	SetLocationHeight(HeightPos);
	RingOriginalRotatingSpeed = RingRotatingSpeed;
	MaterialInterface = Ring_1_Mesh->GetMaterial(1);
	DynamicMaterial = UMaterialInstanceDynamic::Create(MaterialInterface, this);
	Ring_1_Mesh->SetMaterial(1, DynamicMaterial);
	Ring_2_Mesh->SetMaterial(1, DynamicMaterial);
	Ring_3_Mesh->SetMaterial(1, DynamicMaterial);

	bCanBeKnockup = false;
}

void ARGX_DistanceAngel::MoveToTarget(float DeltaTime, FVector TargetPos)
{
	Super::MoveToTarget(DeltaTime, TargetPos);
	SetLocationHeight(HeightPos);
}



void ARGX_DistanceAngel::RotateToTarget(float DeltaTime)
{
	if (TargetActor)
	{
		Super::RotateToTarget(DeltaTime);
		const FVector MyLocation = GetEyeWorldLocation();
		const FVector TargetLocation = TargetActor->GetActorLocation();
		const FRotator RotOffset = UKismetMathLibrary::FindLookAtRotation(MyLocation, TargetLocation);
		FRotator NewRotation = FMath::Lerp(this->GetActorRotation(), RotOffset, DeltaTime * InterpSpeed);
		SphereCollider->SetWorldRotation(NewRotation);
	}
}

void ARGX_DistanceAngel::RotateRings(float DeltaTime) 
{
	const float ClampedDT = DeltaTime > 0.016 ? 0.016 : DeltaTime; //Clamped to a dt of 60 fps
	const float speed = RingRotatingSpeed * ClampedDT;
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
	const FVector ReturnLocation = FloorReturnPlace->GetRelativeLocation();
	SphereCollider->SetRelativeLocation(ReturnLocation);
}

void ARGX_DistanceAngel::TPToOriginalHeight()
{
	float NewHeight = HeightPos;
	FHitResult Result;
	const float UpRaySrcOffset = HeightPos / 2.0f;
	const float UpRayEndOffset = HeightPos * 2.0f;
	FVector ActorLocation = GetEyeWorldLocation();
	const FVector UpVector = GetActorUpVector();
	if (GetWorld()->LineTraceSingleByChannel(Result, ActorLocation + UpVector * UpRaySrcOffset, ActorLocation + UpVector * UpRayEndOffset, ECollisionChannel::ECC_WorldStatic))
	{
		NewHeight = Result.ImpactPoint.Z + ActorMidHeight;
	}
	ActorLocation.Z = NewHeight;
	SphereCollider->SetWorldLocation(ActorLocation);
}

void ARGX_DistanceAngel::SetLocationHeight(float Height) {
	FVector NewLocation = GetActorLocation();
	NewLocation.Z = Height;
	SetActorLocation(NewLocation);

}

void ARGX_DistanceAngel::Tick(float DeltaTime)
{
	if (TickMe == false) return;
	Super::Tick(DeltaTime);
	RotateRings(DeltaTime);
}

float ARGX_DistanceAngel::GetDistanceToTarget() const
{
	return FVector::Distance(GetActorLocation(), TargetActor->GetActorLocation());
}

void ARGX_DistanceAngel::DestroyMyself(float Time)
{
	TickMe = false;
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this] {this->Destroy(); }, Time, false);
}

void ARGX_DistanceAngel::ChangeEyeColor(FLinearColor Color)
{
	DynamicMaterial->SetVectorParameterValue("Color", Color);
}

FVector ARGX_DistanceAngel::GetEyeWorldLocation()
{
	return GetTransform().TransformPosition(SphereCollider->GetRelativeLocation());
}
