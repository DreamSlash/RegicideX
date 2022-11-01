
#include "RGX_SpearProjectile.h"
#include "Components/MCV_AbilitySystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "RegicideX/Components/RGX_HitboxComponent.h"
#include "Kismet/GameplayStatics.h"

ARGX_SpearProjectile::ARGX_SpearProjectile()
{
	SetActorTickEnabled(true);
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.SetTickFunctionEnable(true);
	PrimaryActorTick.bStartWithTickEnabled = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetRelativeLocation(FVector(0.0f));
	StaticMeshComponent->SetupAttachment(RootScene);

	HitboxComponent = CreateDefaultSubobject<URGX_HitboxComponent>(TEXT("Hitbox"));
	HitboxComponent->SetupAttachment(RootScene);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

void ARGX_SpearProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ARGX_SpearProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bWasLaunched == true)
		return;
	
	// TODO: Meter en una funcion
	FVector CasterLocation = Instigator->GetActorLocation();
	FVector CasterRight = Instigator->GetActorRightVector();
	FVector CasterForward = Instigator->GetActorForwardVector();

	FVector CasterOffset = FVector(0.0f);
	CasterOffset = CasterRight.RotateAngleAxis(Angle, CasterForward) * DistanceFromCaster;

	FVector NewLocation = CasterLocation + CasterOffset;

	SetActorLocation(NewLocation);

	FRotator NewRotation = Instigator->GetActorRotation();
	SetActorRotation(NewRotation);
}

void ARGX_SpearProjectile::LaunchProjectile(const AActor* target)
{
	bWasLaunched = true;
	SetLifeSpan(RemainingSeconds);

	if (target)
	{
		FVector SpearLocation = GetActorLocation();
		FVector TargetLocation = target->GetActorLocation();
		FVector Direction = TargetLocation - SpearLocation;
		Direction.Normalize();

		float ProjectileSpeed = ProjectileMovementComponent->GetMaxSpeed();
		FVector WorldVelocity = Direction * ProjectileSpeed;

		FTransform Transform = GetActorTransform();
		FVector LocalVelocity = Transform.InverseTransformVectorNoScale(WorldVelocity);

		ProjectileMovementComponent->SetVelocityInLocalSpace(LocalVelocity);
	}
	else
	{
		float ProjectileSpeed = ProjectileMovementComponent->GetMaxSpeed();
		FVector WorldVelocity = GetActorForwardVector() * ProjectileSpeed;

		FTransform Transform = GetActorTransform();
		FVector LocalVelocity = Transform.InverseTransformVectorNoScale(WorldVelocity);

		ProjectileMovementComponent->SetVelocityInLocalSpace(LocalVelocity);
	}
}

void ARGX_SpearProjectile::HandleHit_Implementation(AActor* OtherActor)
{
}

void ARGX_SpearProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpule, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Manuela this is a bullet\n"));
}

bool ARGX_SpearProjectile::IsHostile(const IGameplayTagAssetInterface* InstigatorTagInterface, const IGameplayTagAssetInterface* OtherTagInterface) const
{
	return false;
}