
#include "RGX_PoolSpawner.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Math/Box.h"

// Sets default values
ARGX_PoolSpawner::ARGX_PoolSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
	RootComponent = SpawnVolume;

	ActorPool = CreateDefaultSubobject<URGX_PoolComponent>(TEXT("ActoorPool"));

}

// Called when the game starts or when spawned
void ARGX_PoolSpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnBox = SpawnBox.ShiftBy(GetActorLocation());
	GetWorldTimerManager().SetTimer(SpawnCooldownTimerHandle, this, &ARGX_PoolSpawner::Spawn, SpawnCooldown, false);
}

void ARGX_PoolSpawner::Spawn()
{
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	ARGX_PoolActor* PooledActor = ActorPool->GetPooledActor();
	if (PooledActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot spawn pooled actor"));
		GetWorldTimerManager().SetTimer(SpawnCooldownTimerHandle, this, &ARGX_PoolSpawner::Spawn, SpawnCooldown, false);
		return;
	}

	const FVector Location = FMath::RandPointInBox(SpawnBox) + FVector(150.0f, 150.0f, 300.0f);

	PooledActor->SetActorLocation(Location);
	PooledActor->SetLifeSpan(LifeSpan);
	PooledActor->SetActive(true);
	PooledActor->SetActorEnableCollision(true);
	GetWorldTimerManager().SetTimer(SpawnCooldownTimerHandle, this, &ARGX_PoolSpawner::Spawn, SpawnCooldown, false);
}

