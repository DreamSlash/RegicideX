// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/AI/Tasks/RGX_BTTask_OrbitAroundTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "RegicideX/Actors/RGX_CharacterBase.h"
#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

EBTNodeResult::Type URGX_BTTask_OrbitAroundTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) 
{
	UE_LOG(LogTemp, Warning, TEXT("Execute Orbit Task. Orbit Time: %f"), CurrentOrbitTime);

	Super::ExecuteTask(OwnerComp, NodeMemory);

	AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr) return EBTNodeResult::Failed;

	APawn* ControlledPawn = AIController->GetPawn();
	if (ControlledPawn == nullptr) return EBTNodeResult::Failed;

	Enemy = Cast<ARGX_EnemyBase>(ControlledPawn);
	if (Enemy == nullptr) return EBTNodeResult::Failed;

	Enemy->GetCharacterMovement()->MaxWalkSpeed = Enemy->OrbitSpeed;

	OrbitTime = UKismetMathLibrary::RandomFloatInRange(OrbitMinTime, OrbitMaxTime);
	OrbitDirection = UKismetMathLibrary::RandomBool() ? 1.0f : -1.0f;

	//bCreateNodeInstance = true;
	bNotifyTick = true;
	return EBTNodeResult::InProgress;
}


void URGX_BTTask_OrbitAroundTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ARGX_CharacterBase* TargetCharacter = Cast<ARGX_CharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetCharacterKey.SelectedKeyName));

	if (TargetCharacter == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	const float OrbitSpeed = Enemy->OrbitSpeed;

	const FVector CurrentLocation = Enemy->GetActorLocation();
	const FVector TargetCharacterLocation = TargetCharacter->GetActorLocation();

	const float DistanceToTarget = FVector::Distance(CurrentLocation, TargetCharacterLocation);
	FVector MyForward = Enemy->GetActorForwardVector();
	MyForward.Z = 0.0f;
	MyForward.Normalize();

	FTransform MyTransform = Enemy->GetActorTransform();

	// First Translation
	const FVector FirstTranslation = MyForward * DistanceToTarget;
	FVector Location = CurrentLocation + FirstTranslation;
	MyTransform.SetLocation(Location);

	// Yaw rotation in target position
	const float AnglesToRotate = 
		(FMath::Square(OrbitSpeed * DeltaSeconds) - FMath::Square(DistanceToTarget) - FMath::Square(DistanceToTarget)) / 
		(-2 * DistanceToTarget * DistanceToTarget);
	FRotator Rotation = Enemy->GetActorRotation();
	Rotation.Yaw += AnglesToRotate * OrbitDirection;
	MyTransform.SetRotation(Rotation.Quaternion());

	// Second Translation
	FVector NewForward = MyTransform.GetRotation().GetForwardVector();
	NewForward.Z = 0.0f;
	NewForward.Normalize();
	const FVector SecondTranslation = -NewForward * DistanceToTarget;
	Location += SecondTranslation;

	// Set Final Location
	MyTransform.SetLocation(Location);

	const FRotator FinalRotation = MyTransform.GetRotation().Rotator();
	const FVector FinalLocation = FVector(MyTransform.GetLocation().X, MyTransform.GetLocation().Y, CurrentLocation.Z);

	Enemy->SetActorRotation(FinalRotation);

	FVector MoveDirection = FinalLocation - CurrentLocation;
	MoveDirection.Normalize();

	Enemy->AddMovementInput(MoveDirection);

	CurrentOrbitTime += DeltaSeconds;

	if (CurrentOrbitTime > OrbitTime)
	{
		CurrentOrbitTime = 0.0f;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
	}
}

void URGX_BTTask_OrbitAroundTarget::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Enemy->GetCharacterMovement()->MaxWalkSpeed = Enemy->MoveSpeed;

	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
