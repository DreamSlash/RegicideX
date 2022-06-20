
#include "RGX_PoolActor.h"

// Sets default values
ARGX_PoolActor::ARGX_PoolActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetActorEnableCollision(false);
}

void ARGX_PoolActor::SetLifeSpan(float InLifespan)
{
	LifeSpan = InLifespan;
	if (LifeSpan > 0.0f)
	{
		GetWorldTimerManager().SetTimer(LifeSpanTimerHandle, this, &ARGX_PoolActor::Deactivate, LifeSpan, false);
	}
}

void ARGX_PoolActor::SetActive(bool InbActive)
{
	bActive = InbActive;
	SetActorHiddenInGame(!bActive);
}

bool ARGX_PoolActor::IsActive()
{
	return bActive;
}

// Called when the game starts or when spawned
void ARGX_PoolActor::BeginPlay()
{
	Super::BeginPlay();
}

void ARGX_PoolActor::Deactivate()
{
	SetActive(false);
}

// Called every frame
void ARGX_PoolActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

