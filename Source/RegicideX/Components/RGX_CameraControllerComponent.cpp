#include "RGX_CameraControllerComponent.h"

#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RegicideX/Character/RGx_PlayerCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

//#pragma optimize("", off)

// Sets default values for this component's properties
URGX_CameraControllerComponent::URGX_CameraControllerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void URGX_CameraControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner<ARGX_PlayerCharacter>();
	SpringArm->bEnableCameraRotationLag = true;

	OriginalArmLength = SpringArm->TargetArmLength;
}

// Called every frame
void URGX_CameraControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Owner == nullptr)
	{
		return;
	}

	TArray<AActor*> ignoredActors; ignoredActors.Add(GetOwner());
	TArray<AActor*> targets = GetNearbyActorsUsingSphere(ignoredActors);

	CalculateSpringArmDistance(targets, DeltaTime);

	if (bTargetingEnabled)
	{
		UpdateTargeting(targets, DeltaTime);
	}

	//if (GEngine)
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Target Arm Length: %f"), SpringArm->TargetArmLength));

	//DrawDebugSphere(GetWorld(), OwnerPawn->GetActorLocation(), CombatRadius, 16, FColor::Red, false, DeltaTime);
}

void URGX_CameraControllerComponent::CalculateSpringArmDistance(const TArray<AActor*>& Targets, float DeltaTime)
{
	const float distanceByEnemies = CalculateSpringArmDistanceByEnemies(Targets);
	const bool bIsDoingAirCombo = (Owner->bIsFallingDown && Owner->IsAttacking());
	const float newDistance = distanceByEnemies + (bIsDoingAirCombo ? ZoomOutAirCombo : 0.0);

	if (fabs(SpringArm->TargetArmLength - newDistance) > UpdateDistanceOffset)
	{
		//const float res = FMath::InterpEaseInOut(SpringArm->TargetArmLength, newDistance, DeltaTime * CameraSpeed, 0.5);
		const float res = FMath::FInterpTo(SpringArm->TargetArmLength, newDistance, DeltaTime, CameraSpeed);
		SpringArm->TargetArmLength = res;
	}
}

float URGX_CameraControllerComponent::CalculateSpringArmDistanceByEnemies(const TArray<AActor*>& Targets) const
{
	const AActor* player = GetOwner();
	const FVector playerLocation = player->GetActorLocation();
	const FVector cameraLocation = Camera->GetComponentLocation();
	const FVector cameraForward = Camera->GetForwardVector();
	const float cosFOV = cos(Camera->FieldOfView);

	int32 numVisibleEnemies = 0;
	int32 numNotVisibleEnemies = 0;
	for (const AActor* target : Targets)
	{
		const float dot = CalculateDotProduct(playerLocation, cameraForward, target);

		if (dot < cosFOV)
		{
			++numNotVisibleEnemies;
		}
		else
		{
			++numVisibleEnemies;
		}
	}

	const float visibleEnemiesZoomOut = ZoomOutPerVisibleEnemy * numVisibleEnemies;
	const float notVisibleEnemiesZoomOut = ZoomOutPerNotVisibleEnemy * numNotVisibleEnemies;
	const float newDistance = std::min(OriginalArmLength + visibleEnemiesZoomOut + notVisibleEnemiesZoomOut, MaxZoomOut);

	return newDistance;
}

void URGX_CameraControllerComponent::UpdateTargeting(TArray<AActor*>& Targets, float DeltaTime)
{
	if (CurrentTarget.IsValid())
	{
		FRotator desiredRotation = CalculateDesiredRotation();

		APawn* OwnerPawn = GetOwner<APawn>();
		FRotator prevRotation = OwnerPawn->GetController()->GetControlRotation();
		FRotator result = FMath::Lerp(prevRotation, desiredRotation, DeltaTime * CameraSpeed);

		if (prevRotation.Equals(result, 5.0f))
		{
			OwnerPawn->GetController()->SetControlRotation(result);
			SpringArm->SetWorldRotation(result);
		}

		//DrawDebugLine(GetWorld(), DesiredCameraLoc, TargetLocation, FColor::Blue);
	}
	else
	{
		if (CurrentTarget.IsExplicitlyNull() == false)
		{
			SetTarget(nullptr);
		}

		FindTarget(Targets);
	}
}

void URGX_CameraControllerComponent::FindTarget(TArray<AActor*>& Targets)
{
	const FVector cameraLocation = Camera->GetComponentLocation();
	const FVector cameraForward = Camera->GetForwardVector();
	Targets.RemoveAllSwap([this, cameraLocation, cameraForward](AActor* Target) -> bool
		{
			if (acos(CalculateDotProduct(cameraLocation, cameraForward, Target)) > TargetingConeAngle)
			{
				return true;
			}

			return false;
		});

	Targets.Sort([this, cameraLocation, cameraForward](const AActor& TargetLeft, const AActor& TargetRight) -> bool
		{
			const float dotLeft = CalculateDotProduct(cameraLocation, cameraForward, &TargetLeft);
			const float dotRight = CalculateDotProduct(cameraLocation, cameraForward, &TargetRight);

			return dotLeft > dotRight;
		});

	if (Targets.Num() > 0)
	{
		if (ARGX_EnemyBase* enemy = Cast<ARGX_EnemyBase>(Targets[0]))
		{
			float dot = CalculateDotProduct(cameraLocation, cameraForward, enemy);
			UE_LOG(LogTemp, Warning, TEXT("Target Dot %f"), dot);
			SetTarget(enemy);
		}
	}
}

FRotator URGX_CameraControllerComponent::CalculateDesiredRotation() const
{
	const float desiredYaw = CalculateDesiredAngle(DesiredYawAngle);
	const float desiredPitch = CalculateDesiredAngle(DesiredPitchAngle);

	const FVector targetLocation = CurrentTarget->GetActorLocation();
	const FVector playerLocation = SpringArm->GetComponentLocation();

	// [SM] Check better way to play with two altitude levels
	FVector aux = targetLocation - playerLocation;
	aux.Z = 0.0;
	FMatrix matrix = FRotationMatrix::MakeFromX(aux);

	FRotator desiredRotation = matrix.Rotator();
	desiredRotation.Yaw += CalculateFinalYaw(desiredYaw);
	desiredRotation.Pitch += CalculateFinalPitch(desiredPitch);

	return desiredRotation;
}

float URGX_CameraControllerComponent::CalculateDesiredAngle(float DesiredAngle) const
{
	const float desiredAngleSin = FMath::Sin(FMath::DegreesToRadians(DesiredAngle));

	const FVector targetLocation = CurrentTarget->GetActorLocation();
	const FVector playerLocation = SpringArm->GetComponentLocation();

	const float pivotDistance = FVector::Distance(playerLocation, targetLocation);

	const float height = SpringArm->TargetArmLength * desiredAngleSin;
	const float beta = FMath::Asin(height / pivotDistance);

	return FMath::DegreesToRadians(DesiredAngle) + beta; 
}

float URGX_CameraControllerComponent::CalculateFinalYaw(float DesiredYaw) const
{
	const FVector playerLocation = SpringArm->GetComponentLocation();
	const FVector playerUpVector = Owner->GetActorUpVector();

	const FVector cameraLocation = Camera->GetComponentLocation();
	const FVector cameraForward = Camera->GetForwardVector();
	const FVector rightVector = FVector::CrossProduct(Owner->GetActorUpVector(), cameraForward);

	const float dot = CalculateDotProduct(playerLocation, rightVector, CurrentTarget.Get());
	const float angle = FMath::RadiansToDegrees(FMath::Acos(dot));
	const float realAngle = 90.0 - angle;

	float finalYaw = FMath::Clamp(FMath::RadiansToDegrees(DesiredYaw), MinYawAngle, MaxYawAngle);
	if (dot > 0.0)
	{
		return -finalYaw;
	}
	else
	{
		return finalYaw;
	}

	//float result = 0.0;
	//if (dot > 0.0)
	//{
	//	if (realAngle < MinYawAngle)
	//	{
	//		result = -MinYawAngle;
	//		//desiredRotation.Yaw -= MinYawAngle;

	//	}
	//	else if (realAngle > MaxYawAngle)
	//	{
	//		result = -MaxYawAngle;
	//		desiredRotation.Yaw -= MaxYawAngle;
	//	}
	//	else
	//	{
	//		return GetOwner<APawn>()->GetController()->GetControlRotation();
	//	}
	//}
	//else if (dot <= 0.0)
	//{
	//	if (realAngle < MinYawAngle)
	//	{
	//		desiredRotation.Yaw += MinYawAngle;
	//	}
	//	else if (realAngle > MaxYawAngle)
	//	{
	//		desiredRotation.Yaw += MaxYawAngle;
	//	}
	//	else
	//	{
	//		return GetOwner<APawn>()->GetController()->GetControlRotation();
	//	}
	//}
}

float URGX_CameraControllerComponent::CalculateFinalPitch(float DesiredPitch) const
{
	return -FMath::Clamp(DesiredPitch, MinPitchAngle, MaxPitchAngle);
}

void URGX_CameraControllerComponent::FindTarget()
{
	//FCollisionObjectQueryParams ObjectQueryParams;
	//ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

	//FCollisionQueryParams QueryParams = FCollisionQueryParams::DefaultQueryParam;
	//QueryParams.AddIgnoredActor(GetOwner());

	//const FVector BoxCenter = Camera->GetComponentLocation() + Camera->GetForwardVector() * TargetTraceDistance * 0.5f;
	//const FVector BoxHalfSize(TargetTraceDistance * 0.5f, 100.0f, 100.0f);

	//TArray<FOverlapResult> Overlaps;
	//const bool bFoundTarget = GetWorld()->OverlapMultiByObjectType(
	//	Overlaps,
	//	BoxCenter,
	//	Camera->GetComponentQuat(),
	//	ObjectQueryParams,
	//	FCollisionShape::MakeBox(BoxHalfSize),
	//	QueryParams
	//);

	//if (bFoundTarget)
	//{
	//	Overlaps.Sort([CameraLoc = Camera->GetComponentLocation()](const FOverlapResult& Lhs, const FOverlapResult& Rhs) {
	//		const float LhsDistSq = FVector::DistSquared(Lhs.GetActor()->GetActorLocation(), CameraLoc);
	//		const float RhsDistSq = FVector::DistSquared(Rhs.GetActor()->GetActorLocation(), CameraLoc);
	//		return LhsDistSq < RhsDistSq;
	//	});

	//	if (ARGX_EnemyBase* enemy = Cast<ARGX_EnemyBase>(Overlaps[0].GetActor()))
	//	{
	//		SetTarget(enemy);
	//	}
	//}

	////DrawDebugBox(GetWorld(), BoxCenter, BoxHalfSize, Camera->GetComponentQuat(), bFoundTarget ? FColor::Green : FColor::Red, false, 2.0f);
}

void URGX_CameraControllerComponent::FindNearestTargetUsingSphere(bool RightDirection)
{
	if (CurrentTarget.IsValid() == false)
	{
		return;
	}

	AActor* player = GetOwner();
	AActor* target = CurrentTarget.Get();

	TArray<AActor*> ignoredActors; 
	ignoredActors.Add(player);
	ignoredActors.Add(target);

	TArray<AActor*> targets = GetNearbyActorsUsingSphere(ignoredActors);

	const FVector playerLocation = player->GetActorLocation();
	FVector forwardVector = (target->GetActorLocation() - playerLocation);
	forwardVector.Normalize();

	targets.RemoveAllSwap([this, playerLocation, forwardVector](AActor* Target) -> bool
	{
		if (acos(CalculateDotProduct(playerLocation, forwardVector, Target)) > TargetingConeAngle)
		{
			return true;
		}

		return false;
	});

	const FVector rightVector = FVector::CrossProduct(player->GetActorUpVector(), forwardVector);

	targets.RemoveAllSwap([this, playerLocation, rightVector, RightDirection](AActor* Target) -> bool
	{
		const float dot = CalculateDotProduct(playerLocation, rightVector, Target);
		if (RightDirection && dot > 0.0)
		{
			return false;
		}
		else if (!RightDirection && dot < 0.0)
		{
			return false;
		}

		return true;
	});

	targets.Sort([this, playerLocation, forwardVector](const AActor& TargetLeft, const AActor& TargetRight) -> bool
	{
		const float dotLeft = CalculateDotProduct(playerLocation, forwardVector, &TargetLeft);
		const float dotRight = CalculateDotProduct(playerLocation, forwardVector, &TargetRight);

		return dotLeft > dotRight;
	});

	if (targets.Num() > 0)
	{
		if (ARGX_EnemyBase* enemy = Cast<ARGX_EnemyBase>(targets[0]))
		{
			float dot = CalculateDotProduct(playerLocation, forwardVector, enemy);
			UE_LOG(LogTemp, Warning, TEXT("Target Dot %f"), dot);
			SetTarget(enemy);
		}
	}

	bFindNearestTargetExecuted = true;
}

TArray<AActor*> URGX_CameraControllerComponent::GetNearbyActorsUsingSphere(const TArray<AActor*>& IgnoredActors) const
{
	const AActor* player = GetOwner();
	const FVector playerLocation = player->GetActorLocation();

	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes; traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	UClass* targetClass = ARGX_EnemyBase::StaticClass();

	TArray<AActor*> targets;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), playerLocation, CombatRadius, traceObjectTypes, targetClass, IgnoredActors, targets);

	return targets;
}

float URGX_CameraControllerComponent::CalculateDotProduct(const FVector& SourceLocation, const FVector& SourceDir, const AActor* target) const
{
	const FVector targetLocation = target->GetActorLocation();
	const FVector targetDirection = targetLocation - SourceLocation;
	const FVector targetDirectionNormalized = targetDirection.GetUnsafeNormal();

	return FVector::DotProduct(SourceDir, targetDirectionNormalized);
}

void URGX_CameraControllerComponent::ToggleTargeting()
{
	if (bTargetingEnabled)
	{
		bTargetingEnabled = false;
		SetTarget(nullptr);
	}
	else
	{
		bTargetingEnabled = true;
	}
}

void URGX_CameraControllerComponent::EnableTargeting()
{
	bTargetingEnabled = true;
}

void URGX_CameraControllerComponent::DisableTargeting()
{
	bTargetingEnabled = false;
	SetTarget(nullptr);
}

void URGX_CameraControllerComponent::CheckYawInput(float Rate)
{
	if (bTargetingEnabled)
	{
		if (bFindNearestTargetExecuted)
		{
			const float absRate = abs(Rate);
			if (absRate < 0.1)
			{
				bFindNearestTargetExecuted = false;
			}
		}
		else
		{
			if (Rate > 0.8)
			{
				FindNearestTargetUsingSphere(true);
			}
			else if (Rate < -0.8)
			{
				FindNearestTargetUsingSphere(false);
			}
		}
	}
}

void URGX_CameraControllerComponent::TargetLeft()
{
	if (bTargetingEnabled)
	{
		FindNearestTargetUsingSphere(false);
	}
}

void URGX_CameraControllerComponent::TargetRight()
{
	if (bTargetingEnabled)
	{
		FindNearestTargetUsingSphere(true);
	}
}

void URGX_CameraControllerComponent::SetTarget(ARGX_EnemyBase* NewTarget)
{
	CurrentTarget = NewTarget;

	if (CurrentTarget.IsValid())
	{
		SpringArm->bUsePawnControlRotation = false;
	}
	else
	{
		SpringArm->bUsePawnControlRotation = true;
	}

	OnTargetUpdated.Broadcast(CurrentTarget.Get());
}
