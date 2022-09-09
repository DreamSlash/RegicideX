// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_LaserTellActorSequence.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ARGX_LaserTellActorSequence::ARGX_LaserTellActorSequence()
{

	PrimaryActorTick.bCanEverTick = true;

	CenterOfMass = CreateDefaultSubobject<USceneComponent>(TEXT("CenterOfMass"));
	CenterOfMass->SetupAttachment(RootComponent);
	
	Particle1 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle1")); Particles.Add(Particle1);
	Particle1->SetupAttachment(RootComponent);
	Particle2 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle2")); Particles.Add(Particle2);
	Particle2->SetupAttachment(RootComponent);
	Particle3 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle3")); Particles.Add(Particle3);
	Particle3->SetupAttachment(RootComponent);
	Particle4 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle4")); Particles.Add(Particle4);
	Particle4->SetupAttachment(RootComponent);
	
	
	CenterOfMass->SetRelativeLocation(FVector(0, 0, 0));
}

// Called when the game starts or when spawned
void ARGX_LaserTellActorSequence::BeginPlay()
{
	Super::BeginPlay();
}

void ARGX_LaserTellActorSequence::ScaleParticles(float DeltaTime)
{
	FVector NewScale = Particle1->GetComponentScale();
	if (NewScale.X > MaxScale)
	{
		bMaxScale = true;
		return;
	}
	NewScale += FVector(ScaleSpeed * DeltaTime);
	for (UParticleSystemComponent* Particle : Particles)
	{
		Particle->SetWorldScale3D(NewScale);
	}
}

void ARGX_LaserTellActorSequence::MoveToCenter(float DeltaTime)
{
	FVector GoalPos = CenterOfMass->GetComponentLocation();
	if(FVector::Distance(Particle1->GetComponentLocation(), GoalPos) < 0.5)
	{
		bAtCenter = true;
		return;
	}
	for (UParticleSystemComponent* Particle : Particles)
	{
		FVector CurrentLocation = Particle->GetComponentLocation();
		FVector NewLocation = FMath::Lerp(CurrentLocation, GoalPos, DeltaTime * InterpSpeed);
		Particle->SetWorldLocation(NewLocation);
	}
}

void ARGX_LaserTellActorSequence::InitParticlePositions()
{
	int i = 0;
	for (UParticleSystemComponent* Particle : Particles)
	{
		FVector NewLocation = InitialParticlesLocations[i];
		Particle->SetWorldLocation(NewLocation);
		i++;
	}
	Particle1->SetWorldScale3D(FVector(0));
	Particle2->SetWorldScale3D(FVector(0));
	Particle3->SetWorldScale3D(FVector(0));
	Particle4->SetWorldScale3D(FVector(0));
}

// Called every frame
void ARGX_LaserTellActorSequence::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 
	CenterOfMass->SetWorldLocation(InitialCenterLocation);
	if (Iterations == 0) 
	{
		InitParticlePositions();
		Iterations++;
	}
	if (!bMaxScale)
	{
		ScaleParticles(DeltaTime);
	}
	else 
	{
		MoveToCenter(DeltaTime);
	}
}

