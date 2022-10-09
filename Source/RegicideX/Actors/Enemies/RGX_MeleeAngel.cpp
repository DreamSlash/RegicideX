// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_MeleeAngel.h"

#include "Kismet/KismetMathLibrary.h"

#include "RegicideX/Components/RGX_MovementAssistComponent.h"
#include "Components/MCV_AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "RegicideX/Components/RGX_HitboxComponent.h"

#include "RegicideX/GAS/RGX_GameplayEffectContext.h"
#include "RegicideX/GAS/RGX_PayloadObjects.h"

ARGX_MeleeAngel::ARGX_MeleeAngel()
{
	MovementAssistComponent = CreateDefaultSubobject<URGX_MovementAssistComponent>(TEXT("MovementAssistComponent"));
	BHHitboxComponent = CreateDefaultSubobject<URGX_HitboxComponent>(TEXT("BHHitboxComponent"));
	BHHitboxComponent->SetupAttachment(RootComponent);

	TornadoSphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("TornadoSphereCollider"));
	TornadoSphereCollider->SetupAttachment(BHHitboxComponent);
	BHHitboxComponent->DeactivateHitbox();
	BHHitboxComponent->OnHitboxOverlap.AddDynamic(this, &ARGX_MeleeAngel::ApplyTornadoEffects);
}


void ARGX_MeleeAngel::RotateToTarget(float DeltaTime)
{

	if (TargetActor)
	{
		const FVector MyLocation = this->GetActorLocation();
		const FVector TargetLocation = TargetActor->GetActorLocation();

		FVector ToTarget = TargetLocation - MyLocation;
		ToTarget.Normalize();
		ToTarget.Z = 0.0f;

		FRotator MyTargetRotation = UKismetMathLibrary::MakeRotFromX(ToTarget);
		MyTargetRotation.Pitch = 0.0f;
		MyTargetRotation.Roll = 0.0f;

		const FRotator NewRotation = FMath::Lerp(this->GetActorRotation(), MyTargetRotation, DeltaTime * RotationInterpSpeed);
		this->SetActorRotation(NewRotation);
	}

}

void ARGX_MeleeAngel::SetGravityScale(float value)
{
	GetCharacterMovement()->GravityScale = value;
}

void ARGX_MeleeAngel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ARGX_MeleeAngel::GetVelocity() const
{
	FVector Velocity = Super::GetVelocity();
	if (Velocity.Size() < 11.f)
	{
		return ChargeVelocity;
	}
	else
	{
		return Velocity;
	}
}

void ARGX_MeleeAngel::ApplyTornadoEffects(AActor* OtherActor)
{
	UAbilitySystemComponent* SourceACS = AbilitySystemComponent;
	UAbilitySystemComponent* TargetACS = OtherActor->FindComponentByClass<UAbilitySystemComponent>();
	if (SourceACS && TargetACS)
	{
		FGameplayEffectContextHandle ContextHandle = SourceACS->MakeEffectContext();
		FRGX_GameplayEffectContext* RGXContext = static_cast<FRGX_GameplayEffectContext*>(ContextHandle.Get());

		for (FRGX_EffectContextContainer& EffectContextContainer : TornadoEffectsToApply)
		{
			if (ensureMsgf(EffectContextContainer.EffectToApply.Get(), TEXT("[Error] OnHitboxOverlap: %s Effect was nullptr"), *GetName()))
			{
				RGXContext->OptionalObject = EffectContextContainer.Payload;
				SourceACS->ApplyGameplayEffectToTarget(EffectContextContainer.EffectToApply->GetDefaultObject<UGameplayEffect>(), TargetACS, this->GetCharacterLevel(), ContextHandle);
			}
		}
	}
}
