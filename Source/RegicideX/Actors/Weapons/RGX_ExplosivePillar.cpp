// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_ExplosivePillar.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "GameplayTags.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "RegicideX/Character/RGX_PlayerCharacter.h"

// Sets default values
ARGX_ExplosivePillar::ARGX_ExplosivePillar()
	: ARGX_EffectApplierActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	PillarCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("PillarCollider"));
	PillarCollider->SetupAttachment(RootComponent);

	ActivationCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ActivationCollider"));
	ActivationCollider->SetupAttachment(RootComponent);

	ExplosionSource = CreateDefaultSubobject<USceneComponent>(TEXT("ExplosionSource"));
	ExplosionSource->SetupAttachment(RootComponent);

	ActivationCollider->OnComponentBeginOverlap.AddUniqueDynamic(this, &ARGX_ExplosivePillar::Activate);
	PillarCollider->OnComponentBeginOverlap.AddUniqueDynamic(this, &ARGX_ExplosivePillar::Detonate);
}

// Called when the game starts or when spawned
void ARGX_ExplosivePillar::BeginPlay()
{
	Super::BeginPlay();

	if (RiseCurve)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TellVFX, GetActorLocation(), GetActorRotation(), FVector(1.0f, 1.0f, 1.0f));

		FOnTimelineFloat TimelineCallback;
		FOnTimelineEventStatic TimelineFinishedCallback;
		TimelineFinishedCallback.BindLambda([this]()
			{
				GetWorld()->GetTimerManager().SetTimer(ActivationTimerHandle, [this]() { Activate(); }, TimeToActivate, false);
				DrawDebugCapsule(GetWorld(), ActivationCollider->GetComponentLocation(), ActivationCollider->GetScaledCapsuleHalfHeight(), ActivationCollider->GetScaledCapsuleRadius(), ActivationCollider->GetComponentRotation().Quaternion(), FColor::Yellow, false, TimeToActivate);
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

void ARGX_ExplosivePillar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsRising && RiseCurve)
	{
		RiseTimeLine.TickTimeline(DeltaTime);

		FVector location = GetActorLocation();
		location.Z = InitialZ + RiseCurve->GetFloatValue(RiseTimeLine.GetPlaybackPosition());
		SetActorLocation(location);
	}
}

void ARGX_ExplosivePillar::Activate(UPrimitiveComponent* OverlappedComponent
	, AActor* OtherActor
	, UPrimitiveComponent* OtherComp
	, int32 OtherBodyIndex
	, bool bFromSweep
	, const FHitResult& SweepResult)
{
	if (IsPlayerOverlapping(OtherActor))
	{
		// If is already activated, do nothing
		if (ExplosionTimerHandle.IsValid() == false)
		{
			GetWorld()->GetTimerManager().ClearTimer(ActivationTimerHandle);
			Activate();
		}
	}
}

void ARGX_ExplosivePillar::Detonate(UPrimitiveComponent* OverlappedComponent
	, AActor* OtherActor
	, UPrimitiveComponent* OtherComp
	, int32 OtherBodyIndex
	, bool bFromSweep
	, const FHitResult& SweepResult)
{
	if (IsPlayerOverlapping(OtherActor))
	{
		if (ExplosionTimerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(ExplosionTimerHandle);
		}

		Explode();
	}
}

void ARGX_ExplosivePillar::Rise()
{
	if (RiseCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEventStatic TimelineFinishedCallback;
		TimelineFinishedCallback.BindLambda([this]()
			{
				GetWorld()->GetTimerManager().SetTimer(ActivationTimerHandle, [this]() { Activate(); }, TimeToActivate, false);
				DrawDebugCapsule(GetWorld(), ActivationCollider->GetComponentLocation(), ActivationCollider->GetScaledCapsuleHalfHeight(), ActivationCollider->GetScaledCapsuleRadius(), ActivationCollider->GetComponentRotation().Quaternion(), FColor::Yellow, false, TimeToActivate);
			});

		RiseTimeLine.AddInterpFloat(RiseCurve, TimelineCallback);
		RiseTimeLine.SetTimelineFinishedFunc(TimelineFinishedCallback);

		RiseTimeLine.PlayFromStart();

		InitialZ = GetActorLocation().Z;
	}
}

void ARGX_ExplosivePillar::Activate()
{
	GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, [this]() { Explode(); }, TimeToExplode, false);

	DrawDebugSphere(GetWorld(), ExplosionSource->GetComponentLocation(), ExplosionRadius, 16, FColor::Red, false, TimeToExplode);
}

void ARGX_ExplosivePillar::Explode()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes; traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	UClass* targetClass = ARGX_PlayerCharacter::StaticClass();
	TArray<AActor*> ignoredActors;
	TArray<AActor*> targets;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), ExplosionSource->GetComponentLocation(), ExplosionRadius, traceObjectTypes, targetClass, ignoredActors, targets);

	if (targets.Num() > 0)
	{
		if (OnPlayerOverlaps(targets[0]))
		{
			// Do something if player was overlapping
		}
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionVFX, ExplosionSource->GetComponentLocation(), GetActorRotation(), FVector(3.0f, 3.0f, 1.0f));
	Destroy();
}
