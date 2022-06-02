// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_MovementAssistComponent.h"

// Sets default values for this component's properties
URGX_MovementAssistComponent::URGX_MovementAssistComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void URGX_MovementAssistComponent::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void URGX_MovementAssistComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!CheckDistanceToGoalPoint()) return;
	MoveForward(DeltaTime);
}

void URGX_MovementAssistComponent::MoveForward(float DeltaTime)
{
	AActor* Owner = GetOwner();
	const FVector FrontVector = Owner->GetActorForwardVector();
	const FVector NewLocation = Owner->GetActorLocation() + FrontVector * DeltaTime * MovementSpeed;
	Owner->SetActorLocation(NewLocation);
}

bool URGX_MovementAssistComponent::CheckDistanceToGoalPoint()
{
	if(bIsMoving && FVector::Distance(GetOwner()->GetActorLocation(), GoalPoint) > 10.0f)
	{
		return true;
	}
	bIsMoving = false;
	return false;
}

void URGX_MovementAssistComponent::SetMagnitudeAndSpeed(float Magnitude, float Speed)
{
	ForwardMagnitude = Magnitude;
	MovementSpeed = Speed;
	AActor* Owner = GetOwner();
	const FVector FrontVector = Owner->GetActorForwardVector();
	GoalPoint = Owner->GetActorLocation() + FrontVector * ForwardMagnitude;
}

void URGX_MovementAssistComponent::DisableMovementAssist()
{
	bIsMoving = false;
}

void URGX_MovementAssistComponent::EnableMovementAssist()
{
	bIsMoving = true;
}

