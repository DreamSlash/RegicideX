// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_RingWave.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"

// Sets default values
ARGX_RingWave::ARGX_RingWave()
	: AActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetupAttachment(RootComponent);

	StaticMeshComponent->OnComponentBeginOverlap.__Internal_AddUniqueDynamic(this, &ARGX_RingWave::DamageActor, FName("DamageActor"));
}

void ARGX_RingWave::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(RingTimeLife);
}

void ARGX_RingWave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FVector CurrentWorldScale = StaticMeshComponent->GetComponentTransform().GetScale3D();
	const float ScaledAxis = CurrentWorldScale.X + DeltaTime * ScaleSpeed;
	const FVector NewScale(ScaledAxis, ScaledAxis, CurrentWorldScale.Z);

	StaticMeshComponent->SetWorldScale3D(NewScale);
}

void ARGX_RingWave::DamageActor(UPrimitiveComponent* OverlappedComponent
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
			ASC->ApplyGameplayEffectToSelf(RingWaveEffect->GetDefaultObject<UGameplayEffect>(), 1.0, ASC->MakeEffectContext());
			//ASC->ApplyGameplayEffectToSelf(WallEffect->GetDefaultObject<UGameplayEffect>(), 1.0, ASC->MakeEffectContext());
		}
	}
}
