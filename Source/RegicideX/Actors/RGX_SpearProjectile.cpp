#include "RGX_SpearProjectile.h"
#include "../Components/RGX_HitboxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Actor.h"

ARGX_SpearProjectile::ARGX_SpearProjectile()
{
	SetActorTickEnabled(true);
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.SetTickFunctionEnable(true);
	PrimaryActorTick.bStartWithTickEnabled = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetRelativeLocation(FVector(0.0f));
	StaticMeshComponent->SetupAttachment(RootComponent);

	HitboxComponent = CreateDefaultSubobject<URGX_HitboxComponent>(TEXT("Hitbox"));
	HitboxComponent->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
	HitboxComponent->SetSimulatePhysics(true);
	HitboxComponent->OnComponentHit.AddUniqueDynamic(this, &ARGX_SpearProjectile::OnComponentHit);
	HitboxComponent->SetupAttachment(StaticMeshComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

void ARGX_SpearProjectile::BeginPlay()
{
	Super::BeginPlay();
	// Manuela
}

void ARGX_SpearProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bWasLaunched == true)
		return;
	
	FVector CasterLocation = Caster->GetActorLocation();
	FVector CasterRight = Caster->GetActorRightVector();
	FVector CasterForward = Caster->GetActorForwardVector();

	FVector CasterOffset = FVector(0.0f);
	CasterOffset = CasterRight.RotateAngleAxis(Angle, CasterForward) * DistanceFromCaster;

	FVector NewLocation = CasterLocation + CasterOffset;

	SetActorLocation(NewLocation);

	FRotator NewRotation = Caster->GetActorRotation();
	SetActorRotation(NewRotation);
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

void ARGX_SpearProjectile::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	CharacterTeam = TeamID;
}

FGenericTeamId ARGX_SpearProjectile::GetGenericTeamId() const
{
	return CharacterTeam;
}
