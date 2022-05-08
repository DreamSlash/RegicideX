// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_LaserBeamWeapon.h"

#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ARGX_LaserBeamWeapon::ARGX_LaserBeamWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PathSplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("PathSplineComponent"));
	EndPointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EndPointMesh"));
	EndPointParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EndPointParticle"));
}



// Called when the game starts or when spawned
void ARGX_LaserBeamWeapon::BeginPlay()
{
	Super::BeginPlay();
	OwnerActor = GetOwner();
}

void ARGX_LaserBeamWeapon::CheckRayTraces(FVector& NewLocation, const float& DeltaTime)
{
	//Front ray to check if the ray is hitting some actor
	FHitResult FrontRayTraceResult;

	const FVector FrontRaySrc = OwnerActor->GetActorLocation() + OwnerActor->GetActorForwardVector() * 200.0f;

	const FVector FrontRayEndPoint = NewLocation;

	//debug
	UKismetSystemLibrary::DrawDebugPoint(GetWorld(), FrontRaySrc, 22, FColor(255, 0, 255), DeltaTime);
	UKismetSystemLibrary::DrawDebugLine(
		GetWorld(),
		FrontRaySrc,
		FrontRayEndPoint,
		FColor(255, 0, 0),
		DeltaTime,
		5.0f
	);

	if (GetWorld()->LineTraceSingleByChannel(FrontRayTraceResult, FrontRaySrc, FrontRayEndPoint, ECollisionChannel::ECC_WorldStatic))
	{
		AActor* Actor = FrontRayTraceResult.GetActor();
		if (Actor != nullptr) {
			//If actor hitting target, apply damage effect
			UE_LOG(LogTemp, Warning, TEXT("Hitting Actor: %s"), *Actor->GetName());
		}
	}

	//Down ray to mantain endpoint in the ground
	FHitResult DownRayTraceResult;

	const FVector DownRaySrc = NewLocation;

	const FVector DownRayEndPoint = NewLocation + EndPointMesh->GetUpVector() * -1000.0;

	if (GetWorld()->LineTraceSingleByChannel(DownRayTraceResult, DownRaySrc, DownRayEndPoint, ECollisionChannel::ECC_WorldStatic)) {
		FVector ImpactPoint = DownRayTraceResult.ImpactPoint;
		NewLocation.Z = ImpactPoint.Z;
	}

}

void ARGX_LaserBeamWeapon::ComputeNewEndpoint(const float& DeltaTime)
{
	const FVector MyLocation = EndPointMesh->K2_GetComponentLocation();

	const FVector TargetLocation = TargetActor->GetActorLocation();

	if (FollowTarget && FVector::Distance(MyLocation, TargetLocation) <= ForgetDistance) {
		FollowTarget = false;
		FTimerHandle Timerhandle;
		GetWorld()->GetTimerManager().SetTimer(Timerhandle, this, &ARGX_LaserBeamWeapon::SetFollowTargetTrue, ForgetTime, false);
	}

	if (FollowTarget)
	{
		const FRotator RotOffset = UKismetMathLibrary::FindLookAtRotation(MyLocation, TargetLocation);
		EndPointMesh->SetWorldRotation(RotOffset);
	}

	const FVector MyForward = EndPointMesh->GetForwardVector();

	FVector NewLocation = MyLocation + MyForward * RaySpeed * DeltaTime;

	CheckRayTraces(NewLocation, DeltaTime);

	EndPointMesh->SetWorldLocation(NewLocation);
}


void ARGX_LaserBeamWeapon::MoveAndDrawRay(const float& DeltaTime)
{
	ComputeNewEndpoint(DeltaTime);

	const FVector EndPoint = EndPointMesh->K2_GetComponentLocation();

	PathSplineComponent->ClearSplinePoints(true);

	PathSplineComponent->AddSplinePointAtIndex(SourcePoint, 0, ESplineCoordinateSpace::World);
	PathSplineComponent->AddSplinePointAtIndex(EndPoint, 1, ESplineCoordinateSpace::World);

	PathSplineComponent->SetSplinePointType(1, ESplinePointType::CurveClamped);

	if(!PathSplineMeshes.IsValidIndex(0))
	{
		SpawnSplineMesh();
		PathSplineMeshes[0]->SetStartScale(FVector2D(2.f));
		PathSplineMeshes[0]->SetEndScale(FVector2D(2.f));
		//PathSplineMeshes[0]->SetMaterial(0, Material);
	}

	const FVector StartTangent = PathSplineComponent->GetTangentAtSplinePoint(0, ESplineCoordinateSpace::World);
	const FVector EndTangent = PathSplineComponent->GetTangentAtSplinePoint(1, ESplineCoordinateSpace::World);

	PathSplineMeshes[0]->SetStartAndEnd(SourcePoint, StartTangent, EndPoint, EndTangent);
}

void ARGX_LaserBeamWeapon::SetSourcePoint(FVector SP)
{
	SourcePoint = SP;
	EndPointMesh->SetWorldLocation(SP);
}

void ARGX_LaserBeamWeapon::SetOwnerActor(AActor* OA)
{
	OwnerActor = OA;
}

void ARGX_LaserBeamWeapon::SetFollowTargetTrue()
{
	FollowTarget = true;
}





