
#include "RGX_CombatManager.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

#include "RegicideX/Character/RGX_PlayerCharacter.h"

// Sets default values
ARGX_CombatManager::ARGX_CombatManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARGX_CombatManager::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->AddOnActorSpawnedHandler(FOnActorSpawned::FDelegate::CreateUObject(this, &ARGX_CombatManager::OnActorSpawned));
}

void ARGX_CombatManager::Tick(float DeltaTime)
{
	if (Player == nullptr)
	{
		Player = Cast<ARGX_PlayerCharacter>(UGameplayStatics::GetActorOfClass(this, ARGX_PlayerCharacter::StaticClass()));
	}

	//const int32 rand = FMath::RandRange(0, 100);
	if (/*rand < 10 || */FVector::Dist2D(LastPlayerPosition, Player->GetActorLocation()) > InvalidateOffset)
	{
		Invalidate();
		LastPlayerPosition = Player->GetActorLocation();
	}

	Update();
}

void ARGX_CombatManager::Invalidate()
{	
	const FVector playerLocation = Player->GetActorLocation();

	for (FRGX_EnemyCombatItem& item : EnemyCombatItems)
	{
		item.Distance = FVector::Dist2D(playerLocation, item.Enemy->GetActorLocation());
		item.Scoring = item.Distance;
	}

	EnemyCombatItems.Sort([this](const FRGX_EnemyCombatItem& left, const FRGX_EnemyCombatItem& right)
		{
			return left.Scoring < right.Scoring;
		});

	int32 index = 0;
	const int32 nbEnemies = EnemyCombatItems.Num();

	while (index < nbEnemies && index < NbHoldingEnemies)
	{
		FRGX_EnemyCombatItem& item = EnemyCombatItems[index++];
		item.Enemy->SetEnemyAIState(ERGX_EnemyAIState::Holding);
	}

	while (index < nbEnemies && index < (NbWaitingEnemies+NbHoldingEnemies))
	{
		FRGX_EnemyCombatItem& item = EnemyCombatItems[index++];
		item.Enemy->SetEnemyAIState(ERGX_EnemyAIState::Waiting);
	}

	while (index < nbEnemies)
	{
		FRGX_EnemyCombatItem& item = EnemyCombatItems[index++];
		item.Enemy->SetEnemyAIState(ERGX_EnemyAIState::Idling);
	}
}

void ARGX_CombatManager::OnActorSpawned(AActor* actor)
{
	if (ARGX_EnemyBase* enemy = Cast<ARGX_EnemyBase>(actor))
	{
		EnemyCombatItems.Emplace(enemy, 0.0, 0.0);
		Invalidate();
	}
}

void ARGX_CombatManager::Update()
{
	TArray<int32> candidates;
	int32 numAttackers = 0;
	int32 numRecoveries = 0;

	PrepareCandidateData(candidates, numAttackers, numRecoveries);

	const int32 numCandidates = candidates.Num();
	while (numAttackers < numCandidates)
	{
		int32 index = FindNewAttacker(candidates);
		auto& item = EnemyCombatItems[index];
		// assert state holding
		item.Enemy->SetEnemyAIState(ERGX_EnemyAIState::Attacking);

		candidates.RemoveAtSwap(index);
		++numAttackers;
	}
}

void ARGX_CombatManager::PrepareCandidateData(TArray<int32>& candidates, int32& numAttackers, int32& numRecoveries) const
{
	candidates.Empty();
	numAttackers = 0;
	numRecoveries = 0;

	for (int32 index = 0; index < EnemyCombatItems.Num() && index < NbHoldingEnemies; ++index)
	{
		const auto& item = EnemyCombatItems[index];
		switch (item.Enemy->GetEnemyAIState())
		{
		case ERGX_EnemyAIState::Attacking:
			++numAttackers;
			break;
		case ERGX_EnemyAIState::Recovering:
			++numRecoveries;
			break;
		case ERGX_EnemyAIState::Holding:
			candidates.Add(index);
			break;
		default:		
			break;
		}
	}
}

int32 ARGX_CombatManager::FindNewAttacker(const TArray<int32>& candidates) const
{
	const int32 numCandidates = candidates.Num();
	return FMath::RandRange(0, numCandidates-1);
}