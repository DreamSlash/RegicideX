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
	//UE_LOG(LogTemp, Warning, TEXT("Execute Orbit Task. Orbit Time: %f"), CurrentOrbitTime);

	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr) return EBTNodeResult::Failed;

	APawn* ControlledPawn = AIController->GetPawn();
	if (ControlledPawn == nullptr) return EBTNodeResult::Failed;

	ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(ControlledPawn);
	if (Enemy == nullptr) return EBTNodeResult::Failed;

	Enemy->GetCharacterMovement()->MaxWalkSpeed = Enemy->OrbitSpeed;

	// jajaja
	if (OwnerComp.GetBlackboardComponent()->IsValidKey(OwnerComp.GetBlackboardComponent()->GetKeyID(OrbitMinTimeName)) == false ||
		OwnerComp.GetBlackboardComponent()->IsValidKey(OwnerComp.GetBlackboardComponent()->GetKeyID(OrbitMaxTimeName)) == false || 
		OwnerComp.GetBlackboardComponent()->IsValidKey(OwnerComp.GetBlackboardComponent()->GetKeyID(OrbitTimeName)) == false || 
		OwnerComp.GetBlackboardComponent()->IsValidKey(OwnerComp.GetBlackboardComponent()->GetKeyID(OrbitDirectionName)) == false ||
		OwnerComp.GetBlackboardComponent()->IsValidKey(OwnerComp.GetBlackboardComponent()->GetKeyID(CurrentOrbitTimeName)) == false)
	{
		return EBTNodeResult::Failed;
	}

	/*const float OrbitMinTime = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(OrbitMinTimeName);
	const float OrbitMaxTime = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(OrbitMaxTimeName);*/

	// Set orbit time for this task execution
	const float OrbitTime = UKismetMathLibrary::RandomFloatInRange(OrbitMinTime, OrbitMaxTime);
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(OrbitTimeName, OrbitTime);
	// Set orbit direction for this task execution
	const float OrbitDirection = UKismetMathLibrary::RandomBool() ? 1.0f : -1.0f;
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(OrbitDirectionName, OrbitDirection);

	bNotifyTick = true;

	Enemy->Orbiting = true;

	return EBTNodeResult::InProgress;
}


void URGX_BTTask_OrbitAroundTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// ----- POINTER CHECKS -----
	ARGX_CharacterBase* TargetCharacter = Cast<ARGX_CharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
	if (TargetCharacter == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	APawn* ControlledPawn = AIController->GetPawn();
	if (ControlledPawn == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(ControlledPawn);
	if (Enemy == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	
	// ----- ORBIT CALCULATION -----
	const float OrbitSpeed = Enemy->OrbitSpeed;
	const float OrbitDirection = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(OrbitDirectionName);

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

	float CurrentOrbitTime = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(CurrentOrbitTimeName);
	CurrentOrbitTime += DeltaSeconds;
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(CurrentOrbitTimeName, CurrentOrbitTime);

	const float OrbitTime = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(OrbitTimeName);

	Enemy->RotateToTarget(DeltaSeconds);

	if (CurrentOrbitTime > OrbitTime)
	{
		CurrentOrbitTime = 0.0f;
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(CurrentOrbitTimeName, CurrentOrbitTime);
		Enemy->Orbiting = false;
		Enemy->GetCharacterMovement()->MaxWalkSpeed = Enemy->GetCurrentMaxSpeed();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
	}
}

//void URGX_BTTask_OrbitAroundTarget::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
//{
//	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
//
//	AAIController* AIController = OwnerComp.GetAIOwner();
//	if (AIController == nullptr) return;
//
//	APawn* ControlledPawn = AIController->GetPawn();
//	if (ControlledPawn == nullptr) return;
//
//	ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(ControlledPawn);
//	if (Enemy == nullptr) return;
//
//	Enemy->Orbiting = false;
//	Enemy->GetCharacterMovement()->MaxWalkSpeed = Enemy->MoveSpeed;
//}
