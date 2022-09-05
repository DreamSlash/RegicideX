// Fill out your copyright notice in the Description page of Project Settings.



#include "RGX_LaserBeamWeapon.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "NavigationSystem.h"
#include "RegicideX/GAS/RGX_GameplayEffectContext.h"
#include "../RGX_CharacterBase.h"

// Sets default values
ARGX_LaserBeamWeapon::ARGX_LaserBeamWeapon()
{
	PathSplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("PathSplineComponent"));
	EndPointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EndPointMesh"));
	EndPointParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EndPointParticle"));
}

// Called when the game starts or when spawned
void ARGX_LaserBeamWeapon::BeginPlay()
{
	Super::BeginPlay();

	OwnerActor = GetOwner();
	ComputeRayGoal();
}

void ARGX_LaserBeamWeapon::CheckRayTraces(FVector& NewLocation, float DeltaTime)
{
	//Front ray to check if the ray is hitting some actor
	FHitResult FrontRayTraceResult;

	const FVector FrontRaySrc = OwnerActor->GetActorLocation() + OwnerActor->GetActorForwardVector() * 200.0f;
	const FVector FrontRayEndPoint = NewLocation;

	//debug
	/*UKismetSystemLibrary::DrawDebugPoint(GetWorld(), FrontRaySrc, 22, FColor(255, 0, 255), DeltaTime);
	UKismetSystemLibrary::DrawDebugLine(
		GetWorld(),
		FrontRaySrc,
		FrontRayEndPoint,
		FColor(255, 0, 0),
		DeltaTime,
		5.0f
	);*/

	if (GetWorld()->LineTraceSingleByChannel(FrontRayTraceResult, FrontRaySrc, FrontRayEndPoint, ECollisionChannel::ECC_WorldStatic))
	{
		AActor* Actor = FrontRayTraceResult.GetActor(); 
		if (bDamageApplied == false && Actor == TargetActor) {
			//If actor hitting target, apply damage effect
			ApplyEffect(TargetActor);
			bDamageApplied = true;
			/*GetWorld()->GetTimerManager().SetTimer(
				EffectApplyTimerhandle, 
				[this]() { this->ApplyEffect(this->TargetActor); },
				0.5,
				true
			);*/
			/*FTimerHandle Timerhandle;
			GetWorld()->GetTimerManager().SetTimer(
				Timerhandle, 
				[this]() {this->bHittingTarget = false; GetWorld()->GetTimerManager().ClearTimer(EffectApplyTimerhandle); 
				EndPointMesh->SetWorldLocation(OwnerActor->GetActorLocation() + OwnerActor->GetActorForwardVector() * 200.0f); },
				ForgetTime, 
				false
			);*/
		}
	}

	//Down ray to mantain endpoint in the ground
	FHitResult DownRayTraceResult;

	const FVector DownRaySrc = NewLocation;
	const FVector DownRayEndPoint = NewLocation + EndPointMesh->GetUpVector() * -1000.0;

	if (GetWorld()->LineTraceSingleByChannel(DownRayTraceResult, DownRaySrc, DownRayEndPoint, ECollisionChannel::ECC_WorldStatic)) {
		NewLocation.Z = DownRayTraceResult.ImpactPoint.Z;
	}
}

void ARGX_LaserBeamWeapon::CheckDistance()
{
	
	const FVector MyLocation = EndPointMesh->K2_GetComponentLocation();

	if (FollowTarget && FVector::Distance(MyLocation, GoalPoint) <= ForgetDistance) {
		bHittingTarget = true;
		/*FollowTarget = false;
		FTimerHandle Timerhandle;
		GetWorld()->GetTimerManager().SetTimer(
			Timerhandle, 
			[this]() {this->FollowTarget = true;},
			ForgetTime, 
			false
		);*/
	}
}

void ARGX_LaserBeamWeapon::ComputeNewEndpoint(float DeltaTime)
{
	SpeedMult += DeltaTime * 0.1;

	const FVector MyLocation = EndPointMesh->K2_GetComponentLocation();

	if (FollowTarget)
	{
		const FRotator RotOffset = UKismetMathLibrary::FindLookAtRotation(MyLocation, GoalPoint);
		EndPointMesh->SetWorldRotation(RotOffset);
	}

	const FVector MyForward = EndPointMesh->GetForwardVector();
	FVector NewLocation = MyLocation + MyForward * RaySpeed * SpeedMult * DeltaTime;

	if (!bHittingTarget) 
	{
		CheckRayTraces(NewLocation, DeltaTime);
	}
	else 
	{
		NewLocation = GoalPoint;
	}
		
	EndPointMesh->SetWorldLocation(NewLocation);
}


void ARGX_LaserBeamWeapon::MoveAndDrawRay(float DeltaTime)
{
	RaySpeed += DeltaTime * RaySpeedMultiplier;

	CheckDistance();
	ComputeNewEndpoint(DeltaTime);

	const FVector EndPoint = EndPointMesh->K2_GetComponentLocation();

	FVector PathIncrement = EndPoint - SourcePoint; PathIncrement /= ParticlesNumber;

	PathSplineComponent->ClearSplinePoints(true);

	PathSplineComponent->AddSplinePointAtIndex(SourcePoint, 0, ESplineCoordinateSpace::World);
	PathSplineComponent->AddSplinePointAtIndex(EndPoint, 1, ESplineCoordinateSpace::World);

	PathSplineComponent->SetSplinePointType(1, ESplinePointType::CurveClamped);

	if(!PathSplineMeshes.IsValidIndex(0))
	{
		SpawnSplineMesh();
		PathSplineMeshes[0]->SetStartScale(FVector2D(2.f));
		PathSplineMeshes[0]->SetEndScale(FVector2D(2.f));
	}

	const FVector StartTangent = PathSplineComponent->GetTangentAtSplinePoint(0, ESplineCoordinateSpace::World);
	const FVector EndTangent = PathSplineComponent->GetTangentAtSplinePoint(1, ESplineCoordinateSpace::World);

	//PathSplineMeshes[0]->SetStartAndEnd(SourcePoint, StartTangent, EndPoint, EndTangent);

	FVector ParticleLocation = SourcePoint;
	for (AActor* ParticleActor : RayParticlesActor) {
		ParticleLocation += PathIncrement;
		ParticleActor->SetActorLocation(ParticleLocation);
	}
}

void ARGX_LaserBeamWeapon::SetSourcePoint(FVector SP)
{
	SourcePoint = SP;
	EndPointMesh->SetWorldLocation(SP);
}

void ARGX_LaserBeamWeapon::ComputeRayGoal()
{
	GoalPoint = TargetActor->GetActorLocation() + OwnerActor->GetActorForwardVector() * GoalDistance;
	UKismetSystemLibrary::DrawDebugPoint(GetWorld(), GoalPoint, 22, FColor(255, 0, 255), 10.0f);
}

void ARGX_LaserBeamWeapon::SetOwnerActor(AActor* OA)
{
	OwnerActor = OA;
}

void ARGX_LaserBeamWeapon::ApplyEffect(AActor* OtherActor)
{
	UAbilitySystemComponent* SourceACS = OwnerActor->FindComponentByClass<UAbilitySystemComponent>();
	UAbilitySystemComponent* TargetACS = OtherActor->FindComponentByClass<UAbilitySystemComponent>();
	ARGX_CharacterBase* MyOwner = Cast<ARGX_CharacterBase>(OwnerActor);
	if (SourceACS && TargetACS)
	{
		FGameplayEffectContextHandle ContextHandle = SourceACS->MakeEffectContext();
		FRGX_GameplayEffectContext* RGXContext = static_cast<FRGX_GameplayEffectContext*>(ContextHandle.Get());

		for (FRGX_EffectContextContainer& EffectContextContainer : EffectToApplyContextContainer)
		{
			if (ensureMsgf(EffectContextContainer.EffectToApply.Get(), TEXT("[Error] %s Effect was nullptr"), *GetName()))
			{
				RGXContext->OptionalObject = EffectContextContainer.Payload;
				SourceACS->ApplyGameplayEffectToTarget(EffectContextContainer.EffectToApply->GetDefaultObject<UGameplayEffect>(), TargetACS, MyOwner->GetCharacterLevel(), ContextHandle);
			}
		}
	}
}