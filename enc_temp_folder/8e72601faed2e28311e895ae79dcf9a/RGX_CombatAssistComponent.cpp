#include "RGX_CombatAssistComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "../Actors/Enemies/RGX_EnemyBase.h"
#include "GameFramework/Actor.h"
#include "Math/UnrealMathUtility.h"

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

	NumEnemiesInsideFrustum = 0;

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
	UpdateTarget();
}

void URGX_CombatAssistComponent::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void URGX_CombatAssistComponent::UpdateTarget()
{
	// Check nearby potential targets
	// TODO: Do not check this every frame.
	AActor* PlayerActor = GetOwner();

	const FVector PlayerLocation = PlayerActor->GetActorLocation();

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	UClass* SeekClass = ARGX_EnemyBase::StaticClass();

	TArray<AActor*> IgnoreActors;
	TArray<AActor*> OutActors;

	ARGX_EnemyBase* NewTarget = nullptr;

	TArray<AActor*> PotentialTargets = GetClosestEnemiesInRange(AutoAssistCloseRadius);
	NewTarget = GetFrontEnemy(PotentialTargets);

	for (AActor* Actor : PotentialTargets)
	{
		ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(Actor);
		if (Enemy->IsInFrustum() == true)
		{
			NumEnemiesInsideFrustum++;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Num enemies in frustum: %d\n"), NumEnemiesInsideFrustum);

	if (NewTarget)
	{
		SetNewTarget(NewTarget);
		return;
	}

	PotentialTargets = GetClosestEnemiesInRange(AutoAssistMaxRadius);
	NewTarget = GetFrontEnemy(PotentialTargets);

	if (NewTarget)
	{
		SetNewTarget(NewTarget);
	}
	else if (Target)
	{
		Target->HideCombatTargetWidget();
		Target = nullptr;
	}

	return;
}

TArray<AActor*> URGX_CombatAssistComponent::GetClosestEnemiesInRange(const float Range) const
{
	// Check nearby potential targets
	// TODO: Do not check this every frame.
	AActor* PlayerActor = GetOwner();
	const FVector PlayerLocation = PlayerActor->GetActorLocation();

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	UClass* SeekClass = ARGX_EnemyBase::StaticClass();

	TArray<AActor*> IgnoreActors;
	TArray<AActor*> OutActors;

	// Check for nearby enemies
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), PlayerLocation, Range, TraceObjectTypes, SeekClass, IgnoreActors, OutActors);

	return OutActors;
}

ARGX_EnemyBase* URGX_CombatAssistComponent::GetFrontEnemy(const TArray<AActor*>& Enemies)
{
	if (Enemies.Num() == 0)
	{
		return nullptr;
	}

	const AActor* PlayerActor = GetOwner();
	const FVector PlayerLocation = PlayerActor->GetActorLocation();	
	const FVector PlayerForward = PlayerActor->GetActorForwardVector();

	ARGX_EnemyBase* FrontEnemy = nullptr;
	TArray<FRGX_ActorAngle> FrontEnemies;
	float SmallestAngle = INFINITY;
	for (AActor* Actor : Enemies)
	{
		const FVector EnemyLocation = Actor->GetActorLocation();;
		FVector EnemyDirection = EnemyLocation - PlayerLocation;
		EnemyDirection.Z = 0.0f;
		EnemyDirection.Normalize();

		const float OrientationToEnemy = -atan2f(EnemyDirection.Y, EnemyDirection.X);
		const float EnemyDeg = FMath::RadiansToDegrees(OrientationToEnemy);

		const FVector ForwardPoint = PlayerForward;
		const float OrientationToForward = -atan2f(ForwardPoint.Y, ForwardPoint.X);
		const float ForwardDeg = FMath::RadiansToDegrees(OrientationToForward);

		float Angle = FMath::RadiansToDegrees(OrientationToForward) - FMath::RadiansToDegrees(OrientationToEnemy);

		const float OrientationSum = FMath::Abs(FMath::RadiansToDegrees(OrientationToForward)) + FMath::Abs(FMath::RadiansToDegrees(OrientationToEnemy));

		if (OrientationSum > 180.0f)
		{ 
			float fAngle = ForwardDeg > 0 ? ForwardDeg : 360.0f + ForwardDeg;
			float eAngle = EnemyDeg > 0 ? EnemyDeg : 360.0f + EnemyDeg;

			Angle = fAngle - eAngle;
		}

		const float AbsAngle = FMath::Abs(Angle);

		//UE_LOG(LogTemp, Warning, TEXT("Angle To Enemy: %f\n"), FMath::RadiansToDegrees(OrientationToEnemy));
		//UE_LOG(LogTemp, Warning, TEXT("Angle To Forward: %f\n"), FMath::RadiansToDegrees(OrientationToForward));
		//UE_LOG(LogTemp, Warning, TEXT("Angle: %f\n"), Angle);

		if (AbsAngle < SmallestAngle)
		{
			SmallestAngle = AbsAngle;
			FrontEnemy = Cast<ARGX_EnemyBase>(Actor);
		}

		FRGX_ActorAngle ActorAngle;
		ActorAngle.Actor = Actor;
		ActorAngle.Angle = AbsAngle;
		FrontEnemies.Add(ActorAngle);
	}

	if (FrontEnemies.Num() == 1)
		return FrontEnemy;

	FrontEnemies.Sort();

	return FrontEnemy;
}

void URGX_CombatAssistComponent::SetNewTarget(ARGX_EnemyBase* NewTarget)
{
	if (Target)
	{
		Target->HideCombatTargetWidget();
	}

	NewTarget->ShowCombatTargetWidget();
	Target = NewTarget;
}

// TODO [REFACTOR]: Autoassist should give extra movement to the attack being done and not be a teleport
void URGX_CombatAssistComponent::PerformAttackAutoAssist()
{
	if (Target == nullptr)
		return;

	AActor* PlayerActor = GetOwner();
	const FVector PlayerLocation = PlayerActor->GetActorLocation();

	const FVector PlayerToEnemyVector = Target->GetActorLocation() - PlayerLocation;
	FRotator Rotation = UKismetMathLibrary::MakeRotFromX(PlayerToEnemyVector);
	Rotation.Pitch = 0.0f;
	Rotation.Roll = 0.0f;

	PlayerActor->SetActorRotation(Rotation);

	const float DistanceToEnemy = FVector::Dist(PlayerLocation, Target->GetActorLocation());

	if (DistanceToEnemy > AutoAssistOffsetToEnemy == false)
		return;

	FVector AssistDirection = FVector(PlayerToEnemyVector.X, PlayerToEnemyVector.Y, 0.0f);
	AssistDirection.Normalize();

	AutoAssistMove = DistanceToEnemy - AutoAssistOffsetToEnemy;

	if (AutoAssistMove > MaxAutoassistMove)
	{
		AutoAssistMove = MaxAutoassistMove;
	}

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
