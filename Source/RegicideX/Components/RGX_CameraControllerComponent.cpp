#include "RGX_CameraControllerComponent.h"

#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"

#pragma optimize("", off)

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
	SpringArm->bEnableCameraRotationLag = true;

	// ...

}

// Called every frame
void URGX_CameraControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const float CameraAngleToTarget = 15.0f;
	const float CameraAngleSin = FMath::Sin(FMath::DegreesToRadians(CameraAngleToTarget));

	if (CurrentTarget.IsValid())
	{
		const FVector& TargetLocation = CurrentTarget->GetActorLocation();
		const FVector& PlayerLocation = SpringArm->GetComponentLocation();

		const float PivotDistance = FVector::Distance(PlayerLocation, TargetLocation);

		const float Height = SpringArm->TargetArmLength * CameraAngleSin;
		const float Beta = FMath::Asin(Height / PivotDistance);
		const float DesiredPitch = FMath::DegreesToRadians(CameraAngleToTarget) + Beta;

		FRotator Rotation = FRotationMatrix::MakeFromX(TargetLocation - PlayerLocation).Rotator();
		Rotation.Yaw -= FMath::RadiansToDegrees(DesiredPitch);

		//FRotator Rotation = FRotationMatrix::MakeFromXZ((TargetLocation - SpringArm->GetComponentLocation()), FVector::UpVector).Rotator();
		//FRotator Rotation = FRotationMatrix::MakeFromXY((TargetLocation - PlayerLocation).GetSafeNormal(), FVector::RightVector).Rotator();
		//Rotation.Pitch = -FMath::RadiansToDegrees(DesiredPitch);
		/*Rotation.Yaw = FMath::RadiansToDegrees(DesiredPitch);*/

		if (APawn* OwnerPawn = GetOwner<APawn>()) {
			OwnerPawn->GetController()->SetControlRotation(Rotation);
		}

		SpringArm->SetWorldRotation(Rotation);

		//DrawDebugLine(GetWorld(), DesiredCameraLoc, TargetLocation, FColor::Blue);
	}
	else
	{
		SetTarget(nullptr);
	}
}

void URGX_CameraControllerComponent::ToggleTargetting()
{
	/*if (CurrentTarget.IsValid()) {
		SetTarget(nullptr);
	}
	else {
		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

		FCollisionQueryParams QueryParams = FCollisionQueryParams::DefaultQueryParam;
		QueryParams.AddIgnoredActor(GetOwner());

		const FVector BoxCenter = Camera->GetComponentLocation() + Camera->GetForwardVector() * TargetTraceDistance * 0.5f;
		const FVector BoxHalfSize(TargetTraceDistance * 0.5f, 100.0f, 100.0f);

		TArray<FOverlapResult> Overlaps;


		const bool bFoundTarget = GetWorld()->OverlapMultiByObjectType(
			Overlaps,
			BoxCenter,
			Camera->GetComponentQuat(),
			ObjectQueryParams,
			FCollisionShape::MakeBox(BoxHalfSize),
			QueryParams
		);

		if (bFoundTarget)
		{
			Overlaps.Sort([CameraLoc = Camera->GetComponentLocation()](const FOverlapResult& Lhs, const FOverlapResult& Rhs) {
				const float LhsDistSq = FVector::DistSquared(Lhs.GetActor()->GetActorLocation(), CameraLoc);
				const float RhsDistSq = FVector::DistSquared(Rhs.GetActor()->GetActorLocation(), CameraLoc);
				return LhsDistSq < RhsDistSq;
			});

			SetTarget(Overlaps[0].GetActor());
		}

		DrawDebugBox(GetWorld(), BoxCenter, BoxHalfSize, Camera->GetComponentQuat(), bFoundTarget ? FColor::Green : FColor::Red, false, 2.0f);
	}*/
}

void URGX_CameraControllerComponent::SetTarget(const ARGX_EnemyBase* NewTarget)
{
	CurrentTarget = NewTarget;
	SpringArm->bUsePawnControlRotation = !CurrentTarget.IsValid();
	//if (!NewTarget) 
	//{
	//	SpringArm->TargetArmLength = OriginalArmLength;
	//}
	//else 
	//{
	//	OriginalArmLength = SpringArm->TargetArmLength;
	//}
}
