// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_MeleeAngel.h"

#include "Kismet/KismetMathLibrary.h"

ARGX_MeleeAngel::ARGX_MeleeAngel()
{

}


void ARGX_MeleeAngel::RotateToTarget(float DeltaTime)
{
	if (TargetActor)
	{
		const FVector MyLocation = this->GetActorLocation();
		const FVector TargetLocation = TargetActor->GetActorLocation();
		const FRotator RotOffset = UKismetMathLibrary::FindLookAtRotation(MyLocation, TargetLocation);
		FRotator NewRotation = FMath::Lerp(this->GetActorRotation(), RotOffset, DeltaTime * InterpSpeed);
		this->SetActorRotation(NewRotation);
	}
}

void ARGX_MeleeAngel::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

}