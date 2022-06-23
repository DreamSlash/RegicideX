
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

	//SpawnBox = CreateDefaultSubobject<FBox>(TEXT("Spawn Box"));

	MeleePeasantPool	= CreateDefaultSubobject<URGX_PoolComponent>(TEXT("MeleePeasantPool"));
	DistancePeasantPool = CreateDefaultSubobject<URGX_PoolComponent>(TEXT("DistancePeasantPool"));
	MolePeasantPool		= CreateDefaultSubobject<URGX_PoolComponent>(TEXT("MolePeasantPool"));
	SuicidePeasantPool	= CreateDefaultSubobject<URGX_PoolComponent>(TEXT("SuicidePeasantPool"));
}

// Called when the game starts or when spawned
void ARGX_PoolSpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnBox = SpawnBox.ShiftBy(GetActorLocation());
	//GetWorldTimerManager().SetTimer(SpawnCooldownTimerHandle, this, &ARGX_PoolSpawner::Spawn, SpawnCooldown, false);
}

ARGX_EnemyBase* ARGX_PoolSpawner::Spawn(ERGX_EnemyTypes Type)
{
	ARGX_EnemyBase* PooledActor = nullptr;
	switch (Type)
	{
	case ERGX_EnemyTypes::MeleePeasant:
		PooledActor = Cast<ARGX_EnemyBase>(MeleePeasantPool->GetPooledActor());
		break;
	case ERGX_EnemyTypes::DistancePeasant:
		PooledActor = Cast<ARGX_EnemyBase>(DistancePeasantPool->GetPooledActor());
		break;
	case ERGX_EnemyTypes::MolePeasant:
		PooledActor = Cast<ARGX_EnemyBase>(MolePeasantPool->GetPooledActor());
		break;
	case ERGX_EnemyTypes::SuicidePeasant:
		PooledActor = Cast<ARGX_EnemyBase>(SuicidePeasantPool->GetPooledActor());
		break;
	default:
		break;
	}
	
	if (PooledActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot spawn pooled actor"));
		//GetWorldTimerManager().SetTimer(SpawnCooldownTimerHandle, this, &ARGX_PoolSpawner::Spawn, SpawnCooldown, false);
		return PooledActor;
	}

	const FVector Location = FMath::RandPointInBox(SpawnBox) + FVector(150.0f, 150.0f, 300.0f);

	PooledActor->SetActorLocation(Location);
	PooledActor->SetLifeSpan(LifeSpan);
	PooledActor->Activate();
	PooledActor->SetActorEnableCollision(true);

	return PooledActor;
	//GetWorldTimerManager().SetTimer(SpawnCooldownTimerHandle, this, &ARGX_PoolSpawner::Spawn, SpawnCooldown, false);
}

