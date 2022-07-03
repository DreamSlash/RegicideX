// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/Actors/Projectiles/RGX_GuidedProjectile.h"
#include "AbilitySystemComponent.h"
#include "RegicideX/Actors/RGX_CharacterBase.h"
#include "RegicideX/GAS/RGX_GameplayEffectContext.h"
#include "Components/StaticMeshComponent.h"
#include "RegicideX/Components/RGX_HitboxComponent.h"
#include "Kismet/KismetMathLibrary.h"

ARGX_GuidedProjectile::ARGX_GuidedProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	HitboxComponent = CreateDefaultSubobject<URGX_HitboxComponent>(TEXT("HitboxComponent"));

	RootComponent = ProjectileMesh;
	HitboxComponent->SetupAttachment(RootComponent);
}

void ARGX_GuidedProjectile::BeginPlay()
{
	Super::BeginPlay();

	HitboxComponent->OnHitboxOverlap.AddDynamic(this, &ARGX_GuidedProjectile::OnHitboxOverlap);
}

void ARGX_GuidedProjectile::Tick(float DeltaTime)
{
	CheckDistance();
	RotateToTarget(DeltaTime);
	Super::Tick(DeltaTime);
}


void ARGX_GuidedProjectile::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	HitboxComponent->OnHitboxOverlap.RemoveDynamic(this, &ARGX_GuidedProjectile::OnHitboxOverlap);
}

void ARGX_GuidedProjectile::OnHitboxOverlap(AActor* OverlappedActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Guided Projectile Overlap\n"));
	ARGX_CharacterBase* CharacterBaseInstigator = Cast<ARGX_CharacterBase>(Instigator);
	if (CharacterBaseInstigator == nullptr)
		return;

	UAbilitySystemComponent* SourceACS = Instigator->FindComponentByClass<UAbilitySystemComponent>();
	UAbilitySystemComponent* TargetACS = OverlappedActor->FindComponentByClass<UAbilitySystemComponent>();
	if (SourceACS && TargetACS)
	{
		FGameplayEffectContextHandle ContextHandle = SourceACS->MakeEffectContext();
		FRGX_GameplayEffectContext* RGXContext = static_cast<FRGX_GameplayEffectContext*>(ContextHandle.Get());
		RGXContext->DamageAmount = Damage;
		RGXContext->ScalingAttributeFactor = 0.0f;

		for (TSubclassOf<UGameplayEffect>& Effect : EffectsToApply)
		{
			if (ensureMsgf(Effect.Get(), TEXT("[Error] OnHitboxOverlap: %s Effect was nullptr"), *GetName()))
			{
				SourceACS->ApplyGameplayEffectToTarget(Effect->GetDefaultObject<UGameplayEffect>(), TargetACS, CharacterBaseInstigator->GetCharacterLevel(), ContextHandle);
			}
		}
	}
}

void ARGX_GuidedProjectile::RotateToTarget(float DeltaTime)
{
	if (TargetActor && bStopFollowing == false)
	{
		const FVector MyLocation = this->GetActorLocation();
		const FVector TargetLocation = TargetActor->GetActorLocation();
		const FRotator RotOffset = UKismetMathLibrary::FindLookAtRotation(MyLocation, TargetLocation);
		this->SetActorRotation(RotOffset);
	}
}

void ARGX_GuidedProjectile::CheckDistance()
{
	const FVector MyLocation = this->GetActorLocation();
	const FVector TargetLocation = TargetActor->GetActorLocation();

	const  float Dist = FVector::Distance(MyLocation, TargetLocation);
	if (Dist <= StopFollowingDistance)
	{
		bStopFollowing = true;
	}
}
