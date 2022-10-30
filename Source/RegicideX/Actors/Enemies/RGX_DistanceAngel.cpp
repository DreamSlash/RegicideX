// Fill out your copyright notice in the Description page of Project Settings.

#include "RGX_DistanceAngel.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "Components/MCV_AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "Math/UnrealMathUtility.h"

#include "Components/WidgetComponent.h"
#include "RegicideX/Components/RGX_HitboxComponent.h"
#include "RegicideX/GAS/RGX_GameplayEffectContext.h"

ARGX_DistanceAngel::ARGX_DistanceAngel() : ARGX_EnemyBase()
{
	Ring_2_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ring2"));
	ForceFieldSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ForceFieldSphere"));

	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));

	FloorReturnPlace = CreateDefaultSubobject<USceneComponent>(TEXT("FloorReturnPlace"));

	SphereCollider->SetupAttachment(RootComponent);
	GetMesh()->SetupAttachment(SphereCollider);
	Ring_2_Mesh->SetRelativeLocation(FVector(0.0));
	Ring_2_Mesh->SetupAttachment(GetMesh());
	ForceFieldSphere->SetupAttachment(SphereCollider);

	FloorReturnPlace->SetRelativeLocation(FVector(0.0));
	FloorReturnPlace->SetupAttachment(RootComponent);

	CombatTargetWidgetComponent->SetupAttachment(SphereCollider);

	BHHitboxComponent = CreateDefaultSubobject<URGX_HitboxComponent>(TEXT("BHHitboxComponent"));
	BHHitboxComponent->SetupAttachment(SphereCollider);

	ForceFieldSphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("ForceFieldSphereCollider"));
	ForceFieldSphereCollider->SetupAttachment(BHHitboxComponent);
	ForceFieldSphere->SetHiddenInGame(true);

	BHHitboxComponent->DeactivateHitbox();
	BHHitboxComponent->OnHitboxOverlap.AddDynamic(this, &ARGX_DistanceAngel::ApplyForceFieldEffects);

	SetActorEnableCollision(true);

}

void ARGX_DistanceAngel::BeginPlay()
{
	Super::BeginPlay();
	SetLocationHeight(HeightPos);
	RingOriginalRotatingSpeed = RingRotatingSpeed;
	MaterialInterface = Ring_2_Mesh->GetMaterial(1);
	DynamicMaterial = UMaterialInstanceDynamic::Create(MaterialInterface, this);
	Ring_2_Mesh->SetMaterial(1, DynamicMaterial);

	bCanBeKnockup = false;
}

void ARGX_DistanceAngel::MoveToTarget(float DeltaTime, FVector TargetPos)
{
	Super::MoveToTarget(DeltaTime, TargetPos);
	SetLocationHeight(HeightPos);
}

void ARGX_DistanceAngel::RotateToTarget(float DeltaTime)
{
	if (TargetActor)
	{
		Super::RotateToTarget(DeltaTime);
		const FVector MyLocation = GetEyeWorldLocation();
		const FVector TargetLocation = TargetActor->GetActorLocation();
		const FRotator RotOffset = UKismetMathLibrary::FindLookAtRotation(MyLocation, TargetLocation);
		FRotator NewRotation = FMath::Lerp(this->GetActorRotation(), RotOffset, DeltaTime * RotationInterpSpeed);
		SphereCollider->SetWorldRotation(NewRotation);
	}
}

void ARGX_DistanceAngel::ForceRotateToTarget()
{
	if (TargetActor)
	{
		const FVector MyLocation = GetEyeWorldLocation();
		const FVector TargetLocation = TargetActor->GetActorLocation();
		const FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(MyLocation, TargetLocation);
		SphereCollider->SetWorldRotation(NewRotation);
	}
}

void ARGX_DistanceAngel::RotateRings(float DeltaTime) 
{
	const float ClampedDT = DeltaTime > 0.016 ? 0.016 : DeltaTime; //Clamped to a dt of 60 fps
	const float speed = RingRotatingSpeed * ClampedDT;
	Ring_2_Mesh->AddLocalRotation(FRotator(-speed, 0.0, 0.0));
}

void ARGX_DistanceAngel::RotateMe(float DeltaTime, float Speed)
{
	FRotator NewRotation(0.0);
	NewRotation.Yaw += FullBodyRotatingSpeed * DeltaTime * Speed;
	GetMesh()->SetWorldRotation(NewRotation);
}

void ARGX_DistanceAngel::TPToFloor()
{
	const FVector ReturnLocation = FloorReturnPlace->GetRelativeLocation();
	SphereCollider->SetRelativeLocation(ReturnLocation);
}

void ARGX_DistanceAngel::TPToOriginalHeight()
{
	float NewHeight = HeightPos;
	FHitResult Result;
	const float UpRaySrcOffset = HeightPos / 2.0f;
	const float UpRayEndOffset = HeightPos * 2.0f;
	FVector ActorLocation = GetEyeWorldLocation();
	const FVector UpVector = GetActorUpVector();
	if (GetWorld()->LineTraceSingleByChannel(Result, ActorLocation + UpVector * UpRaySrcOffset, ActorLocation + UpVector * UpRayEndOffset, ECollisionChannel::ECC_WorldStatic))
	{
		NewHeight = Result.ImpactPoint.Z - ActorMidHeight;
	}
	ActorLocation.Z = NewHeight;
	SphereCollider->SetWorldLocation(ActorLocation);
}

void ARGX_DistanceAngel::SetLocationHeight(float Height) {
	FVector NewLocation = GetActorLocation();
	NewLocation.Z = Height;
	SetActorLocation(NewLocation);

}

void ARGX_DistanceAngel::Tick(float DeltaTime)
{
	if (TickMe == false) return;
	Super::Tick(DeltaTime);
	RotateRings(DeltaTime);
}

float ARGX_DistanceAngel::GetDistanceToTarget() const
{
	return FVector::Distance(GetActorLocation(), TargetActor->GetActorLocation());
}

void ARGX_DistanceAngel::DestroyMyself(float Time)
{
	TickMe = false;
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this] {this->Destroy(); }, Time, false);
}

void ARGX_DistanceAngel::ChangeEyeColor(FLinearColor Color)
{
	DynamicMaterial->SetVectorParameterValue("Color", Color);
}

FVector ARGX_DistanceAngel::GetEyeWorldLocation()
{
	return GetTransform().TransformPosition(SphereCollider->GetRelativeLocation());
}

void ARGX_DistanceAngel::HandleDamage(
	float DamageAmount,
	const FHitResult& HitInfo,
	const struct FGameplayTagContainer& DamageTags,
	ARGX_CharacterBase* InstigatorCharacter,
	AActor* DamageCauser,
	ERGX_AnimEvent HitReactFlag)
{
	Super::HandleDamage(DamageAmount, HitInfo, DamageTags, InstigatorCharacter, DamageCauser, HitReactFlag);

	if (IsAlive() == false)
	{
		AAIController* AiController = Cast<AAIController>(GetController());
		if(AiController)
			AiController->GetBrainComponent()->StopLogic(FString("Character has died."));

		SphereCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (LaserBeamRef != nullptr)
		{
			LaserBeamRef->Destroy();
		}

		Ring_2_Mesh->SetSimulatePhysics(true);
		PrimaryActorTick.bCanEverTick = false;
		DestroyMyself(22.0f);

		HandleDeath();
	}
}

void ARGX_DistanceAngel::ApplyForceFieldEffects(AActor* OtherActor)
{
	UAbilitySystemComponent* SourceACS = AbilitySystemComponent; 
	UAbilitySystemComponent* TargetACS = OtherActor->FindComponentByClass<UAbilitySystemComponent>();
	if (SourceACS && TargetACS)
	{
		FGameplayEffectContextHandle ContextHandle = SourceACS->MakeEffectContext();
		FRGX_GameplayEffectContext* RGXContext = static_cast<FRGX_GameplayEffectContext*>(ContextHandle.Get());

		for (FRGX_EffectContextContainer& EffectContextContainer : ForceFieldEffectsToApply)
		{
			if (ensureMsgf(EffectContextContainer.EffectToApply.Get(), TEXT("[Error] OnHitboxOverlap: %s Effect was nullptr"), *GetName()))
			{
				RGXContext->OptionalObject = EffectContextContainer.Payload;
				SourceACS->ApplyGameplayEffectToTarget(EffectContextContainer.EffectToApply->GetDefaultObject<UGameplayEffect>(), TargetACS, this->GetCharacterLevel(), ContextHandle);
			}
		}
	}
	//BHHitboxComponent->DeactivateHitbox();
}

void ARGX_DistanceAngel::HandleDeath()
{
	Super::HandleDeath();
	OnHandleDeath();
}
