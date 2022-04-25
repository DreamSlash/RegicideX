// Fill out your copyright notice in the Description page of Project Settings.

#include "RGX_DistanceAngel.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Components/SphereComponent.h"



ARGX_DistanceAngel::ARGX_DistanceAngel() 
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


	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ARGX_DistanceAngel::RotateToTarget(float DeltaTime)
{

	if (TargetActor)
	{

		FVector MyLocation = this->GetActorLocation();
		FVector TargetLocation = TargetActor->GetActorLocation();
		FRotator RotOffset = UKismetMathLibrary::FindLookAtRotation(MyLocation, TargetLocation);
		FRotator NewRotation = FMath::Lerp(this->GetActorRotation(), RotOffset, DeltaTime * InterpSpeed);
		this->SetActorRotation(NewRotation);
	}

}


void ARGX_DistanceAngel::MoveToTarget(float DeltaTime, FVector TargetPos)
{

	FVector MyFront = this->GetActorForwardVector();
	MyFront.Normalize();
	FVector CurrentLocation = this->GetActorLocation();
	FVector NewLocation = CurrentLocation + MyFront * MoveSpeed * DeltaTime;
	this->SetActorLocation(NewLocation);

}

void ARGX_DistanceAngel::RotateRings(float DeltaTime) 
{
	float speed = RotatingSpeed * DeltaTime;
	Ring_2_Mesh->AddLocalRotation(FRotator(-speed, 0.0, speed));
	Ring_3_Mesh->AddLocalRotation(FRotator(0.0, speed, speed));
}



void ARGX_DistanceAngel::Tick(float DeltaTime)
{
	RotateToTarget(DeltaTime);
	RotateRings(DeltaTime);
}