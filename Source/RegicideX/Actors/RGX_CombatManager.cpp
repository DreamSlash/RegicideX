
#include "RGX_CombatManager.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

#include "RegicideX/Character/RGX_PlayerCharacter.h"

#pragma optimize("", off)

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

	EnemyMeleeItems.Reserve(MaxMeleeEnemies);
	EnemyMeleeItems.Init(FRGX_EnemyCombatItem(), MaxMeleeEnemies);

	EnemyDistanceItems.Reserve(MaxDistanceEnemies);
	EnemyDistanceItems.Init(FRGX_EnemyCombatItem(), MaxDistanceEnemies);

	GetWorld()->AddOnActorSpawnedHandler(FOnActorSpawned::FDelegate::CreateUObject(this, &ARGX_CombatManager::OnActorSpawned));
}

void ARGX_CombatManager::Tick(float DeltaTime)
{
	if (Player == nullptr)
	{
		Player = Cast<ARGX_PlayerCharacter>(UGameplayStatics::GetActorOfClass(this, ARGX_PlayerCharacter::StaticClass()));
	}

	if (bAddedNewEnemies || bEnemyDead || FVector::Dist2D(LastPlayerPosition, Player->GetActorLocation()) > InvalidateOffset)
	{
		Invalidate();
		LastPlayerPosition = Player->GetActorLocation();
		bAddedNewEnemies = false;
		bEnemyDead = false;
	}

	Update();
}

void ARGX_CombatManager::Invalidate()
{	
	const FVector playerLocation = Player->GetActorLocation();

	for (FRGX_EnemyCombatItem& item : EnemyMeleeItems)
	{
		if (item.IsValid())
		{
			item.Distance = FVector::Dist2D(playerLocation, item.Enemy->GetActorLocation());
			item.Scoring = item.Distance;
		}
	}

	EnemyMeleeItems.Sort([this](const FRGX_EnemyCombatItem& left, const FRGX_EnemyCombatItem& right)
		{
			if (left.IsValid() && right.IsValid())
			{
				return left.Scoring < right.Scoring;
			}
			else if (left.IsValid())
			{
				return true;
			}
			else
			{
				return false;
			}
		});

	int32 index = 0;
	const int32 lastIndex = EnemyMeleeItems.IndexOfByPredicate([](const FRGX_EnemyCombatItem& item) { return item.IsValid() == false; });

	while (index < lastIndex && index < NbHoldingEnemies)
	{
		FRGX_EnemyCombatItem& item = EnemyMeleeItems[index++];
		if (item.Enemy->GetEnemyAIState() == ERGX_EnemyAIState::None || item.Enemy->GetEnemyAIState() == ERGX_EnemyAIState::Waiting)
		{
			item.Enemy->SetEnemyAIState(ERGX_EnemyAIState::Holding);
		}
	}

	while (index < lastIndex)
	{
		FRGX_EnemyCombatItem& item = EnemyMeleeItems[index++];
		if (item.Enemy->GetEnemyAIState() == ERGX_EnemyAIState::None)
		{
			item.Enemy->SetEnemyAIState(ERGX_EnemyAIState::Waiting);
		}
	}
}

void ARGX_CombatManager::OnActorSpawned(AActor* actor)
{
	if (ARGX_EnemyBase* enemy = Cast<ARGX_EnemyBase>(actor))
	{
		int32 index = 0;
		while (index < EnemyMeleeItems.Num())
		{
			auto& item = EnemyMeleeItems[index++];
			if (item.IsValid() == false)
			{
				item.Reset(enemy);
				enemy->OnHandleDeathEvent.AddDynamic(this, &ARGX_CombatManager::OnEnemyDeath);
				bAddedNewEnemies = true;
				return;
			}
		}
	}
}

void ARGX_CombatManager::OnEnemyDeath(ARGX_EnemyBase* enemy)
{
	bEnemyDead = true;
}

void ARGX_CombatManager::Update()
{
	TArray<int32> candidates;
	int32 numAttackers = 0;
	int32 numRecoveries = 0;

	PrepareCandidateData(candidates, numAttackers, numRecoveries);

	const int32 numCandidates = candidates.Num();
	while (numAttackers < numCandidates && numAttackers < NbMeleeSlots)
	{
		int32 index = FindNewAttacker(candidates);
		auto& item = EnemyMeleeItems[index];
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

	const int32 lastIndex = EnemyMeleeItems.IndexOfByPredicate([](const FRGX_EnemyCombatItem& item) { return item.IsValid() == false; });
	for (int32 index = 0; index < lastIndex && index < NbHoldingEnemies; ++index)
	{
		const auto& item = EnemyMeleeItems[index];
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