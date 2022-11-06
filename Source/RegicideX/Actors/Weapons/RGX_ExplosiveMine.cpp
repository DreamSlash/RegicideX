// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_ExplosiveMine.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "GameplayTags.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "RegicideX/Character/RGX_PlayerCharacter.h"

// Sets default values
ARGX_ExplosiveMine::ARGX_ExplosiveMine()
	: ARGX_EffectApplierActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(RootComponent);

	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereCollider->SetupAttachment(Sphere);

	ActivationCollider = CreateDefaultSubobject<USphereComponent>(TEXT("ActivationCollider"));
	ActivationCollider->SetupAttachment(Sphere);
}

// Called when the game starts or when spawned
void ARGX_ExplosiveMine::BeginPlay()
{
	Super::BeginPlay();

	BeginRise();
}

void ARGX_ExplosiveMine::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	Super::EndPlay(EndPlayReason);
}

void ARGX_ExplosiveMine::SetTargetActor(AActor* Target)
{
	TargetActor = Target;
}

void ARGX_ExplosiveMine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TickRise(DeltaTime);
	TickActivation(DeltaTime);
}

void ARGX_ExplosiveMine::TickRise(float DeltaTime)
{
	if (bIsRising && RiseCurve)
	{
		RiseTimeLine.TickTimeline(DeltaTime);

		FVector location = GetActorLocation();
		location.Z = InitialZ + RiseCurve->GetFloatValue(RiseTimeLine.GetPlaybackPosition());
		SetActorLocation(location);
	}
}

void ARGX_ExplosiveMine::TickActivation(float DeltaTime)
{
	if (bIsActivated)
	{
		const FVector CurrentWorldScale = Sphere->GetComponentTransform().GetScale3D();
		const float ScaledAxis = CurrentWorldScale.X + DeltaTime * ActivationScaleSpeed;
		const FVector NewScale(ScaledAxis, ScaledAxis, ScaledAxis);

		Sphere->SetWorldScale3D(NewScale);
	}
}

void ARGX_ExplosiveMine::Activate(UPrimitiveComponent* OverlappedComponent
	, AActor* OtherActor
	, UPrimitiveComponent* OtherComp
	, int32 OtherBodyIndex
	, bool bFromSweep
	, const FHitResult& SweepResult)
{
	if (IsPlayerOverlapping(OtherActor))
	{
		GetWorld()->GetTimerManager().ClearTimer(AutoActivationTimerHandle);
		Activate();
	}
}

void ARGX_ExplosiveMine::Detonate(UPrimitiveComponent* OverlappedComponent
	, AActor* OtherActor
	, UPrimitiveComponent* OtherComp
	, int32 OtherBodyIndex
	, bool bFromSweep
	, const FHitResult& SweepResult)
{
	if (IsPlayerOverlapping(OtherActor))
	{
		GetWorld()->GetTimerManager().ClearTimer(ExplosionTimerHandle);
		Explode();
	}
}

void ARGX_ExplosiveMine::BeginRise()
{
	if (RiseCurve)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TellVFX, GetActorLocation(), GetActorRotation());

		FOnTimelineFloat TimelineCallback;
		FOnTimelineEventStatic TimelineFinishedCallback;
		TimelineFinishedCallback.BindLambda([this]()
			{
				bIsRising = false;

				if (ActivationCollider->IsOverlappingActor(TargetActor))
				{
					Activate();
				}
				else
				{
					ActivationCollider->OnComponentBeginOverlap.AddUniqueDynamic(this, &ARGX_ExplosiveMine::Activate);
					GetWorld()->GetTimerManager().SetTimer(AutoActivationTimerHandle, [this]() { Activate(); }, TimeAutoactivation, false);
				}
				//DrawDebugCapsule(GetWorld(), ActivationCollider->GetComponentLocation(), ActivationCollider->GetScaledCapsuleHalfHeight(), ActivationCollider->GetScaledCapsuleRadius(), ActivationCollider->GetComponentRotation().Quaternion(), FColor::Yellow, false, TimeToActivate);
			});

		RiseTimeLine.AddInterpFloat(RiseCurve, TimelineCallback);
		RiseTimeLine.SetTimelineFinishedFunc(TimelineFinishedCallback);

		InitialZ = GetActorLocation().Z;
		FVector location = GetActorLocation();
		location.Z = InitialZ + RiseCurve->GetFloatValue(RiseTimeLine.GetPlaybackPosition());
		SetActorLocation(location);

		GetWorld()->GetTimerManager().SetTimer(DelayTellTimerHandle, [this]() { RiseTimeLine.PlayFromStart(); bIsRising = true; }, TimeDelayTell, false);
	}
}

void ARGX_ExplosiveMine::BeginActivation()
{
	bIsActivated = true;

	if (ActivationTellVFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ActivationTellVFX, Sphere->GetComponentLocation(), GetActorRotation());
	}
}

void ARGX_ExplosiveMine::Activate()
{
	ActivationCollider->OnComponentBeginOverlap.RemoveAll(this);
	GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, [this]() { Explode(); }, TimeToExplode, false);
	SphereCollider->OnComponentBeginOverlap.AddUniqueDynamic(this, &ARGX_ExplosiveMine::Detonate);
	BeginActivation();
	//DrawDebugSphere(GetWorld(), ExplosionSource->GetComponentLocation(), ExplosionRadius, 16, FColor::Red, false, TimeToExplode);
}

void ARGX_ExplosiveMine::Explode()
{
	bIsActivated = false;

	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes; traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	UClass* targetClass = ARGX_PlayerCharacter::StaticClass();
	TArray<AActor*> ignoredActors;
	TArray<AActor*> targets;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Sphere->GetComponentLocation(), ExplosionRadius, traceObjectTypes, targetClass, ignoredActors, targets);

	if (targets.Num() > 0)
	{
		if (OnPlayerOverlaps(targets[0]))
		{
			// Do something if player was overlapping
		}
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionVFX, Sphere->GetComponentLocation(), GetActorRotation());
	Destroy();
}
