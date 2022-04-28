// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_Bullet.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ARGX_Bullet::ARGX_Bullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletStaticMesh"));
	BulletCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BulletCollider"));
	BulletParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BulletParticles"));
	RootComponent = BulletMesh;
	//BulletMesh->SetupAttachment(RootComponent);

	BulletCollider->SetRelativeLocation(FVector(0.0));
	BulletCollider->SetupAttachment(RootComponent);

	BulletParticleSystem->SetRelativeLocation(FVector(0.0));
	BulletParticleSystem->SetupAttachment(RootComponent);

	BulletCollider->OnComponentBeginOverlap.AddUniqueDynamic(this, &ARGX_Bullet::Hit);

	this->InitialLifeSpan = 2.0f;
}

// Called when the game starts or when spawned
void ARGX_Bullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARGX_Bullet::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	MyTimer += DeltaTime;
	Move(DeltaTime);

}

void ARGX_Bullet::Move(float DeltaTime)
{
	FVector MyFront = this->GetActorForwardVector();
	MyFront.Normalize();
	FVector CurrentLocation = this->GetActorLocation();
	FVector NewLocation = CurrentLocation + MyFront * MoveSpeed * DeltaTime;
	this->SetActorLocation(NewLocation);
}

void ARGX_Bullet::Hit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//do stuff
	if (MyTimer > 0.5 && OtherActor != MyOwner)
	{
		this->Destroy();
	}	
}

