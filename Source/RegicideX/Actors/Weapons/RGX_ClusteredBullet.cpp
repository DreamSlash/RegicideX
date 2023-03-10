
#include "RGX_ClusteredBullet.h"
#include "Components/StaticMeshComponent.h"
#include "RegicideX/Components/RGX_HitboxComponent.h"

// Sets default values
ARGX_ClusteredBullet::ARGX_ClusteredBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene		= CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	BulletCollider	= CreateDefaultSubobject<URGX_HitboxComponent>(TEXT("BulletCollider"));
	BulletMesh		= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));

	BulletCollider->SetupAttachment(RootScene);
	BulletMesh->SetupAttachment(RootScene);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

// Called when the game starts or when spawned
void ARGX_ClusteredBullet::BeginPlay()
{
	Super::BeginPlay();
	
	if(LifeSpan > 0.0f)
		GetWorld()->GetTimerManager().SetTimer(DestroyTimeHandle, this, &ARGX_ClusteredBullet::HandleDeath, LifeSpan, false);
}

void ARGX_ClusteredBullet::HandleDeath()
{
	this->Destroy();
}

void ARGX_ClusteredBullet::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	CharacterTeam = TeamID;
}

FGenericTeamId ARGX_ClusteredBullet::GetGenericTeamId() const
{
	return CharacterTeam;
}

