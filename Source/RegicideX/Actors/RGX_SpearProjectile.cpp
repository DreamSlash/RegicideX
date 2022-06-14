#include "RGX_SpearProjectile.h"
#include "../Components/RGX_HitboxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Enemies/RGX_EnemyBase.h"
#include "Components/MCV_AbilitySystemComponent.h"

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

void ARGX_SpearProjectile::LaunchProjectile()
{
	bWasLaunched = true;
	SetLifeSpan(RemainingSeconds);

	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), EnemyClass, FoundEnemies);
	
	float ClosestEnemyDistance = INFINITY;
	
	for (AActor* Enemy : FoundEnemies)
	{
		ARGX_EnemyBase* EnemyBase = Cast<ARGX_EnemyBase>(Enemy);
		bool bIsDead = EnemyBase->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.Dead")));
		if (bIsDead == true)
			continue;

		FVector SpearLocation = GetActorLocation();
		FVector EnemyLocation = EnemyBase->GetActorLocation();
		float Distance = FVector::Dist(SpearLocation, EnemyLocation);
		if (Distance > TargetMaxRange)
			continue;

		if (Distance < ClosestEnemyDistance)
		{
			ClosestEnemy = EnemyBase;
			ClosestEnemyDistance = Distance;
		}
	}

	if (ClosestEnemy == nullptr)
	{
		FVector Forward = GetActorForwardVector();
		float ProjectileSpeed = ProjectileMovementComponent->GetMaxSpeed();
		FVector WorldVelocity = Forward * ProjectileSpeed;

		FTransform Transform = GetActorTransform();
		FVector LocalVelocity = Transform.InverseTransformVectorNoScale(WorldVelocity);

		ProjectileMovementComponent->SetVelocityInLocalSpace(LocalVelocity);
	}
	else
	{
		FVector SpearLocation = GetActorLocation();
		FVector ClosestEnemyLocation = ClosestEnemy->GetActorLocation();
		FVector Direction = ClosestEnemyLocation - SpearLocation;
		Direction.Normalize();

		float ProjectileSpeed = ProjectileMovementComponent->GetMaxSpeed();
		FVector WorldVelocity = Direction * ProjectileSpeed;

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

void ARGX_SpearProjectile::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	CharacterTeam = TeamID;
}

FGenericTeamId ARGX_SpearProjectile::GetGenericTeamId() const
{
	return CharacterTeam;
}
