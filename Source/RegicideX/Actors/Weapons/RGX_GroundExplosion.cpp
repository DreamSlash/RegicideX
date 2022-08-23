// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_GroundExplosion.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "GameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
ARGX_GroundExplosion::ARGX_GroundExplosion()
	: ARGX_EffectApplierActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	ExplosionCollider = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollider"));
	ExplosionCollider->SetupAttachment(RootComponent);

	CircumferenceDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("CircumferenceDecal"));
	CircumferenceDecal->SetupAttachment(RootComponent);

	AreaDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("AreaDecal"));
	AreaDecal->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ARGX_GroundExplosion::BeginPlay()
{
	Super::BeginPlay();

	CircumRef = CircumferenceDecal->CreateDynamicMaterialInstance();
	CircumRef->SetScalarParameterValue(FName("Circum1Area0"), 1.0);

	AreaRef = AreaDecal->CreateDynamicMaterialInstance();
	AreaRef->SetScalarParameterValue(FName("Circum1Area0"), 0.0);

	if (AreaCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEventStatic TimelineFinishedCallback;
		TimelineFinishedCallback.BindLambda([this]() { Explode(); });

		AreaTimeLine.AddInterpFloat(AreaCurve, TimelineCallback);
		AreaTimeLine.SetTimelineFinishedFunc(TimelineFinishedCallback);

		AreaTimeLine.PlayFromStart();
	}
	//GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle)
}

void ARGX_GroundExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AreaCurve)
	{
		AreaTimeLine.TickTimeline(DeltaTime);

		AreaRef->SetScalarParameterValue(FName("AreaValue"), AreaCurve->GetFloatValue(AreaTimeLine.GetPlaybackPosition()));
	}
}

void ARGX_GroundExplosion::Explode()
{
	TArray<AActor*> OverlappedActors;
	ExplosionCollider->GetOverlappingActors(OverlappedActors, TargetActor);

	for (AActor* Actor : OverlappedActors)
	{
		if (OnPlayerOverlaps(Actor))
		{

		}
		/*if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, true))
		{
			if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("PossessedBy.Player")))
			{
				ASC->ApplyGameplayEffectToSelf(ExplosionEffect->GetDefaultObject<UGameplayEffect>(), 1.0, ASC->MakeEffectContext());
			}
		}*/
	}

	// Fix Z to hit the ground
	FHitResult result;
	FVector location = GetActorLocation();
	DrawDebugLine(GetWorld(), location, location * FVector(1, 1, -1), FColor(255, 0, 0), true, 5.0f, 0, 5.0f);
	if (GetWorld()->LineTraceSingleByChannel(result, location, location * FVector(1, 1, -1), ECollisionChannel::ECC_WorldStatic))
	{
		location.Z = result.ImpactPoint.Z;
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionVFX, location, GetActorRotation(), FVector(3.0f, 3.0f, 1.0f));

	Destroy();
}
