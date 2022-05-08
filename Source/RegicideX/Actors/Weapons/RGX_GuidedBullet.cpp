// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_GuidedBullet.h"

#include "Kismet/KismetMathLibrary.h"


ARGX_GuidedBullet::ARGX_GuidedBullet() : ARGX_Bullet()
{

}

void ARGX_GuidedBullet::BeginPlay()
{
	Super::BeginPlay();
}

void ARGX_GuidedBullet::Tick(float DeltaTime)
{
	CheckDistance();
	RotateToTarget(DeltaTime);
	Super::Tick(DeltaTime);
}

void ARGX_GuidedBullet::RotateToTarget(float DeltaTime)
{
	if (TargetActor && bStopFollowing == false)
	{
		const FVector MyLocation = this->GetActorLocation();
		const FVector TargetLocation = TargetActor->GetActorLocation();
		const FRotator RotOffset = UKismetMathLibrary::FindLookAtRotation(MyLocation, TargetLocation);
		this->SetActorRotation(RotOffset);
	}
}

void ARGX_GuidedBullet::CheckDistance()
{
	const FVector MyLocation = this->GetActorLocation();
	const FVector TargetLocation = TargetActor->GetActorLocation();

	const  float Dist = FVector::Distance(MyLocation, TargetLocation);
	if (Dist <= StopFollowingDistance)
	{
		bStopFollowing = true;
	}
}
