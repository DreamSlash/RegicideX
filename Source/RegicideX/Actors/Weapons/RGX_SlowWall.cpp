// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_SlowWall.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"
#include "GameplayTags.h"

// Sets default values
ARGX_SlowWall::ARGX_SlowWall()
	: AActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	WallCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("WallCollider"));
	WallCollider->SetupAttachment(RootComponent);

	WallCollider->OnComponentBeginOverlap.AddUniqueDynamic(this, &ARGX_SlowWall::SlowActor);
}

// Called when the game starts or when spawned
void ARGX_SlowWall::BeginPlay()
{
	Super::BeginPlay();
	
	if (WallCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEventStatic TimelineFinishedCallback;
		TimelineFinishedCallback.BindLambda([this]()
		{
			GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, [this]() {Destroy(); }, LifeTimeOnceIsRisen, false);
		});

		RiseTimeLine.AddInterpFloat(WallCurve, TimelineCallback);
		RiseTimeLine.SetTimelineFinishedFunc(TimelineFinishedCallback);

		RiseTimeLine.PlayFromStart();

		InitialZ = GetActorLocation().Z;
	}
}

void ARGX_SlowWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (WallCurve)
	{
		RiseTimeLine.TickTimeline(DeltaTime);

		FVector location = GetActorLocation();
		location.Z = InitialZ + WallCurve->GetFloatValue(RiseTimeLine.GetPlaybackPosition());
		SetActorLocation(location);
	}
}

void ARGX_SlowWall::SlowActor(UPrimitiveComponent* OverlappedComponent
	, AActor* OtherActor
	, UPrimitiveComponent* OtherComp
	, int32 OtherBodyIndex
	, bool bFromSweep
	, const FHitResult& SweepResult)
{
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor, true))
	{
		if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("PossessedBy.Player")))
		{
			ASC->ApplyGameplayEffectToSelf(WallEffect->GetDefaultObject<UGameplayEffect>(), 1.0, ASC->MakeEffectContext());
		}
	}
}
