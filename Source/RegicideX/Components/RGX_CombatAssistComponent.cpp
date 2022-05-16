#include "RGX_CombatAssistComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "../Actors/Enemies/RGX_EnemyBase.h"
#include "GameFramework/Actor.h"

URGX_CombatAssistComponent::URGX_CombatAssistComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URGX_CombatAssistComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URGX_CombatAssistComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Extra movement vector (from animation attacks, etc...)
	if (bMoveVectorEnabled && bAddMoveVector)
	{
		AActor* Owner = GetOwner();

		if (Target)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Has Target\n"));
			const float DistanceToTarget = FVector::Distance(Owner->GetActorLocation(), Target->GetActorLocation());

			// Apply attack movement only if it does not get too close to the target
			if (DistanceToTarget > AutoAssistOffsetToEnemy)
			{
				float MoveSpeed = MoveVectorSpeed + AutoAssistMove / AttackMoveDuration;
				//UE_LOG(LogTemp, Warning, TEXT("AutoAssist MoveSpeed: %f\n"), MoveSpeed);
				const FVector NewLocation = Owner->GetActorLocation() + MoveVectorDirection * MoveSpeed * DeltaTime;
				Owner->SetActorLocation(NewLocation, true);
			}
			else if (DistanceToTarget > AttackOffsetToEnemy)
			{
				const float TotalMoveLeft = AttackMoveDurationLeft * MoveVectorSpeed;
				const float MaxMove = DistanceToTarget - AttackOffsetToEnemy;

				if (TotalMoveLeft <= MaxMove)
				{
					//UE_LOG(LogTemp, Warning, TEXT("Inferior to MaxMove Speed: %f\n"), MoveVectorSpeed);
					const FVector NewLocation = Owner->GetActorLocation() + MoveVectorDirection * MoveVectorSpeed * DeltaTime;
					Owner->SetActorLocation(NewLocation, true);
				}
				else
				{
					float MoveSpeed = MaxMove / AttackMoveDurationLeft;
					//UE_LOG(LogTemp, Warning, TEXT("Superior to MaxMove Speed: %f\n"), MoveSpeed);
					const FVector NewLocation = Owner->GetActorLocation() + MoveVectorDirection * MoveSpeed * DeltaTime;
				}
			}
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("Does not have Target\n"));
			FVector FinalVelocity = MoveVectorDirection * MoveVectorSpeed;
			//UE_LOG(LogTemp, Warning, TEXT("Speed: %f, %f, %f\n"), FinalVelocity.X, FinalVelocity.Y, FinalVelocity.Z);
			const FVector NewLocation = Owner->GetActorLocation() + MoveVectorDirection * MoveVectorSpeed * DeltaTime;
			Owner->SetActorLocation(NewLocation, true);
		}
	}

	AttackMoveDurationLeft -= DeltaTime;
	// -----------------------------
}

void URGX_CombatAssistComponent::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// TODO [REFACTOR]: Autoassist should give extra movement to the attack being done and not be a teleport
void URGX_CombatAssistComponent::PerformAttackAutoAssist()
{
	AActor* PlayerActor = GetOwner();

	const FVector PlayerLocation = PlayerActor->GetActorLocation();

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	UClass* SeekClass = ARGX_EnemyBase::StaticClass();

	TArray<AActor*> IgnoreActors;
	TArray<AActor*> OutActors;

	// Check for nearby enemies
	if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), PlayerLocation, AutoAssistRadius, TraceObjectTypes, SeekClass, IgnoreActors, OutActors) == false)
		return;

	float CurrentClosestDistance = INFINITY;
	FVector NearestEnemyLocation = FVector(0.0f, 0.0f, 0.0f);
	bool bHasTarget = false;

	// Check the closest enemy inside a cone in front of the player
	for (AActor* Actor : OutActors)
	{
		ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(Actor);

		const bool bIsDead = Enemy->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.Dead")));

		if (bIsDead == true)
			continue;

		const FVector EnemyLocation = Enemy->GetActorLocation();

		const float Distance = FVector::Dist(PlayerLocation, EnemyLocation);

		FVector PlayerToEnemyVector = EnemyLocation - PlayerLocation;
		PlayerToEnemyVector.Normalize();

		const FVector PlayerForward = PlayerActor->GetActorForwardVector();

		// Cone check
		const float Dot = FVector::DotProduct(PlayerToEnemyVector, PlayerForward);

		if (Distance < CurrentClosestDistance && Dot > AutoAssistDot)
		{
			CurrentClosestDistance = Distance;
			NearestEnemyLocation = EnemyLocation;
			bHasTarget = true;
			Target = Enemy;
		}
	}

	if (bHasTarget == false)
		return;

	const FVector PlayerToEnemyVector = NearestEnemyLocation - PlayerLocation;
	FRotator Rotation = UKismetMathLibrary::MakeRotFromX(PlayerToEnemyVector);
	Rotation.Pitch = 0.0f;
	Rotation.Roll = 0.0f;

	PlayerActor->SetActorRotation(Rotation);

	if (CurrentClosestDistance > AutoAssistOffsetToEnemy == false)
		return;

	FVector AssistDirection = FVector(PlayerToEnemyVector.X, PlayerToEnemyVector.Y, 0.0f);
	AssistDirection.Normalize();

	AutoAssistMove = CurrentClosestDistance - AutoAssistOffsetToEnemy;

	//UE_LOG(LogTemp, Warning, TEXT("AutoAssistMove: %f\n"), AutoAssistMove);
	//const FVector FinalLocation = PlayerLocation + AssistDirection * (CurrentClosestDistance - AutoAssistOffsetToEnemy);

	//PlayerActor->SetActorLocation(FinalLocation);
}

void URGX_CombatAssistComponent::EnableMovementVector()
{
	//UE_LOG(LogTemp, Warning, TEXT("Enable Move Vector\n"));
	bMoveVectorEnabled = true;
}

void URGX_CombatAssistComponent::DisableMovementVector()
{
	//UE_LOG(LogTemp, Warning, TEXT("Disable Move Vector\n"));
	bMoveVectorEnabled = false;
	Target = nullptr; // [TODO]: This is hardcoded.
}

void URGX_CombatAssistComponent::AddMovementVector(FVector Direction, float Speed)
{
	FVector VelocityVector = Direction * Speed;
	//UE_LOG(LogTemp, Warning, TEXT("Add Move Vector: %f, %f, %f\n"), VelocityVector.X, VelocityVector.Y, VelocityVector.Z);
	MoveVectorDirection = Direction;
	MoveVectorSpeed = Speed;
	bAddMoveVector = true;
}

void URGX_CombatAssistComponent::RemoveMovementVector()
{
	//UE_LOG(LogTemp, Warning, TEXT("Remove Move Vector\n"));
	MoveVectorDirection = FVector(0.0f);
	MoveVectorSpeed = 0.0f;
	bAddMoveVector = false;
}

void URGX_CombatAssistComponent::SetAttackMoveDuration(float Duration)
{
	AttackMoveDuration = Duration;
	AttackMoveDurationLeft = AttackMoveDuration;
}
