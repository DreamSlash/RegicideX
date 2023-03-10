// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_Bullet.h"

#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "RegicideX\Components\RGX_HitboxComponent.h"

// Sets default values
ARGX_Bullet::ARGX_Bullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletStaticMesh"));
	BulletCollider = CreateDefaultSubobject<URGX_HitboxComponent>(TEXT("BulletCollider"));
	BulletParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BulletParticles"));
	RootComponent = BulletMesh;

	BulletCollider->SetRelativeLocation(FVector(0.0));
	BulletCollider->SetupAttachment(RootComponent);

	BulletParticleSystem->SetRelativeLocation(FVector(0.0));
	BulletParticleSystem->SetupAttachment(RootComponent);

	//BulletCollider->OnComponentBeginOverlap.AddUniqueDynamic(this, &ARGX_Bullet::Hit);

	this->InitialLifeSpan = 2.0f;
}

// Called when the game starts or when spawned
void ARGX_Bullet::BeginPlay()
{
	Super::BeginPlay();
}

void ARGX_Bullet::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	CharacterTeam = TeamID;
}

FGenericTeamId ARGX_Bullet::GetGenericTeamId() const
{
	return CharacterTeam;
}

// Called every frame
void ARGX_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Move(DeltaTime);
}

void ARGX_Bullet::Move(float DeltaTime)
{
	const FVector MyFront = this->GetActorForwardVector();
	const FVector CurrentLocation = this->GetActorLocation();
	const FVector NewLocation = CurrentLocation + MyFront * MoveSpeed * DeltaTime;

	this->SetActorLocation(NewLocation);
}

void ARGX_Bullet::Hit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetInstigator()) return;

	this->Destroy();
}

