
#include "RGX_PoolComponent.h"

// Sets default values for this component's properties
URGX_PoolComponent::URGX_PoolComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

ARGX_PoolActor* URGX_PoolComponent::GetPooledActor()
{
	for (ARGX_PoolActor* PooledActor : Pool)
	{
		if (PooledActor->IsActive() == false)
			return PooledActor;
	}
	return nullptr;
}


// Called when the game starts
void URGX_PoolComponent::BeginPlay()
{
	Super::BeginPlay();

	if (PooledActorSubclass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			for (int i = 0; i < PoolSize; ++i)
			{
				ARGX_PoolActor* PoolableActor = GetWorld()->SpawnActor<ARGX_PoolActor>(PooledActorSubclass, FVector().ZeroVector, FRotator().ZeroRotator);
				Pool.Add(PoolableActor);
			}
		}
	}
	
}