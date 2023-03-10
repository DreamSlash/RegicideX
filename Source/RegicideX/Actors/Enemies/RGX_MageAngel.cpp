// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_MageAngel.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "RegicideX/AI/Controllers/RGX_MageAngelController.h"

ARGX_MageAngel::ARGX_MageAngel()
	: ARGX_EnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	MageCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MageCapsule"));
	MageCapsule->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	MageCapsule->CanCharacterStepUpOn = ECB_No;
	MageCapsule->SetShouldUpdatePhysicsVolume(true);
	MageCapsule->SetCanEverAffectNavigation(false);
	MageCapsule->bDynamicObstacle = true;
	MageCapsule->SetupAttachment(GetMesh());

	RingWaveSource = CreateDefaultSubobject<USceneComponent>(TEXT("RingWaveSource"));
	RingWaveSource->SetupAttachment(RootComponent);
	SetActorEnableCollision(true);
}

void ARGX_MageAngel::BeginPlay()
{
	Super::BeginPlay();

	// Set the height of the angel at the beginning
	FVector NewLocation = GetActorLocation();
	NewLocation.Z = HeightPos;
	SetActorLocation(NewLocation);

	// Remove gravity from Character Movement Component
	//GetCharacterMovement()->GravityScale = 0.0f;
}

void ARGX_MageAngel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARGX_MageAngel::HandleDamage(float DamageAmount
	, const FHitResult& HitInfo
	, const struct FGameplayTagContainer& DamageTags
	, ARGX_CharacterBase* InstigatorCharacter
	, AActor* DamageCauser
	, ERGX_AnimEvent HitReactFlag)
{
	Super::HandleDamage(DamageAmount, HitInfo, DamageTags, InstigatorCharacter, DamageCauser, HitReactFlag);
}