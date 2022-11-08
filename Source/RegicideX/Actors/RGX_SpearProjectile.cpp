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
	const FVector CasterLocation = Instigator->GetActorLocation();
	const FVector CasterRight = Instigator->GetActorRightVector();
	const FVector CasterForward = Instigator->GetActorForwardVector();

	FVector CasterOffset = FVector(0.0f);
	CasterOffset = CasterRight.RotateAngleAxis(Angle, CasterForward) * DistanceFromCaster;

	SetActorLocation(CasterLocation + CasterOffset);

	SetActorRotation(Instigator->GetActorRotation());
}

void ARGX_SpearProjectile::LaunchProjectile(const AActor* target)
{
	bWasLaunched = true;
	SetLifeSpan(RemainingSeconds);

	FVector WorldVelocity;
	const float ProjectileSpeed = ProjectileMovementComponent->GetMaxSpeed();

	if (target)
	{
		if (Cast<ARGX_EnemyBase>(target))
		{
			const FVector SpearLocation = GetActorLocation();
			const USkeletalMeshComponent* TargetMesh = Cast<ARGX_EnemyBase>(target)->GetMesh();
			const FVector TargetLocation = TargetMesh->GetSkeletalCenterOfMass();
			FVector Direction = TargetLocation - SpearLocation;
			Direction.Normalize();

			WorldVelocity = Direction * ProjectileSpeed;

			ProjectileMovementComponent->HomingTargetComponent = Cast<ARGX_EnemyBase>(target)->TargetingTransform;
		}
		else
		{
			const FVector SpearLocation = GetActorLocation();
			const FVector TargetLocation = target->GetActorLocation();
			FVector Direction = TargetLocation - SpearLocation;
			Direction.Normalize();

			WorldVelocity = Direction * ProjectileSpeed;

			ProjectileMovementComponent->HomingTargetComponent = target->GetRootComponent();
		}
	}
	else
	{
		WorldVelocity = GetActorForwardVector() * ProjectileSpeed;
	}

	LaunchVelocity = GetActorTransform().InverseTransformVectorNoScale(WorldVelocity);
	ProjectileMovementComponent->SetVelocityInLocalSpace(LaunchVelocity);
}

void ARGX_SpearProjectile::HandleHit_Implementation(AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Call to HandleHit_Implementation\n"));
}

void ARGX_SpearProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, FVector NormalImpule, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Call to OnComponentHit\n"));
}

bool ARGX_SpearProjectile::IsHostile(const IGameplayTagAssetInterface* InstigatorTagInterface,
                                     const IGameplayTagAssetInterface* OtherTagInterface) const
{
	return false;
}
