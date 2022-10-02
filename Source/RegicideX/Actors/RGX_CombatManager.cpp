
#include "RGX_CombatManager.h"

#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RegicideX/AI/Controllers/RGX_EnemyBaseController.h"
#include "RegicideX/Character/RGX_PlayerCharacter.h"

//#pragma optimize("", off)

//---- FRGX_EnemyCombatItem -----------------------------------------------------------------------

void FRGX_EnemyCombatItem::Reset(ARGX_EnemyBase* enemy)
{
	if (enemy)
	{
		Enemy = enemy;
		LastAttackTime = UGameplayStatics::GetTimeSeconds(enemy);
	}
	else
	{
		Enemy.Reset();
		LastAttackTime = 0.0;
	}

	Distance = 0.0;
	Scoring = 0.0;
}

//---- ARGX_CombatManager -------------------------------------------------------------------------

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

	EnemyRangedItems.Reserve(MaxRangedEnemies);
	EnemyRangedItems.Init(FRGX_EnemyCombatItem(), MaxRangedEnemies);

	GetWorld()->AddOnActorPreSpawnInitialization(FOnActorSpawned::FDelegate::CreateUObject(this, &ARGX_CombatManager::OnActorSpawned));
	//GetWorld()->AddOnActorSpawnedHandler(FOnActorSpawned::FDelegate::CreateUObject(this, &ARGX_CombatManager::OnActorSpawned));
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

	UpdateMeleeEnemies();
	UpdateDistanceEnemies();
}

void ARGX_CombatManager::Invalidate()
{	
	InvalidateImpl(EnemyMeleeItems);
	InvalidateImpl(EnemyRangedItems);
}

void ARGX_CombatManager::OnActorSpawned(AActor* actor)
{
	if (ARGX_EnemyBase* enemy = Cast<ARGX_EnemyBase>(actor))
	{
		switch (enemy->GetEnemyType())
		{
		case ERGX_EnemyType::MeleePeasant:
		case ERGX_EnemyType::ShieldPeasant:
			OnEnemySpawned(enemy, EnemyMeleeItems);
			break;
		case ERGX_EnemyType::DistancePeasant:
			OnEnemySpawned(enemy, EnemyRangedItems);
			break;
		}
	}
}

void ARGX_CombatManager::OnEnemySpawned(ARGX_EnemyBase* Enemy, TArray<FRGX_EnemyCombatItem>& EnemyList)
{
	int32 index = 0;
	while (index < EnemyList.Num())
	{
		auto& item = EnemyList[index++];
		if (item.IsValid() == false)
		{
			item.Reset(Enemy);

			Enemy->OnHandleDeathEvent.AddDynamic(this, &ARGX_CombatManager::OnEnemyDeath);
			Enemy->TargetActor = Player.Get();

			bAddedNewEnemies = true;
			return;
		}
	}
}

void ARGX_CombatManager::OnEnemyDeath(ARGX_EnemyBase* enemy)
{
	bEnemyDead = true;
}

void ARGX_CombatManager::InvalidateImpl(TArray<FRGX_EnemyCombatItem>& EnemyItems)
{
	const FVector playerLocation = Player->GetActorLocation();

	for (FRGX_EnemyCombatItem& item : EnemyItems)
	{
		if (item.IsValid())
		{
			item.Distance = FVector::Dist2D(playerLocation, item.Enemy->GetActorLocation());
			item.Scoring = item.Distance;
		}
	}

	EnemyItems.Sort([this](const FRGX_EnemyCombatItem& left, const FRGX_EnemyCombatItem& right)
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
	const int32 lastIndex = EnemyItems.IndexOfByPredicate([](const FRGX_EnemyCombatItem& item) { return item.IsValid() == false; });

	while (index < lastIndex && index < NbHoldingEnemies)
	{
		FRGX_EnemyCombatItem& item = EnemyItems[index++];
		if (ARGX_EnemyBaseController* enemyController = item.Enemy->GetController<ARGX_EnemyBaseController>())
		{
			if (enemyController->GetEnemyAIState() == ERGX_EnemyAIState::None || enemyController->GetEnemyAIState() == ERGX_EnemyAIState::Waiting)
			{
				enemyController->SetEnemyAIState(ERGX_EnemyAIState::Holding);
			}
		}
	}

	while (index < lastIndex)
	{
		FRGX_EnemyCombatItem& item = EnemyItems[index++];
		if (ARGX_EnemyBaseController* enemyController = item.Enemy->GetController<ARGX_EnemyBaseController>())
		{
			if (enemyController->GetEnemyAIState() == ERGX_EnemyAIState::None)
			{
				enemyController->SetEnemyAIState(ERGX_EnemyAIState::Waiting);
			}
		}
	}
}

void ARGX_CombatManager::UpdateMeleeEnemies()
{
	const FVector playerLocation = Player->GetActorLocation();
	
	float oldestAttackTime = UGameplayStatics::GetTimeSeconds(this);
	for (FRGX_EnemyCombatItem& item : EnemyMeleeItems)
	{
		if (item.IsValid())
		{
			oldestAttackTime = std::min(oldestAttackTime, item.LastAttackTime);
		}
	}

	UpdateScoring(EnemyMeleeItems, [this, playerLocation, oldestAttackTime](FRGX_EnemyCombatItem& item)
		{
			item.Distance = std::numeric_limits<float>::infinity();
			item.Scoring = std::numeric_limits<float>::infinity();

			if (item.IsValid())
			{
				item.Distance = FVector::Dist2D(playerLocation, item.Enemy->GetActorLocation());
				const float speed = item.Enemy->MoveSpeed;

				const float distanceScore = (item.Distance / speed) * DistanceWeight;
				const float visibilityScore = (item.Enemy->IsInFrustum() ? 0.0 : IsNotInFrustumScore) * IsNotInFrustumWeight;
				const float lastAttackTimeScore = (item.LastAttackTime - oldestAttackTime) * LastAttackTimeWeight;
				
				item.Scoring = distanceScore + visibilityScore + lastAttackTimeScore;
			}			
		});

	UpdateSlots(EnemyMeleeItems, NbMeleeSlots);
}

void ARGX_CombatManager::UpdateDistanceEnemies()
{
	const FVector playerLocation = Player->GetActorLocation();
	
	float oldestAttackTime = UGameplayStatics::GetTimeSeconds(this);
	for (FRGX_EnemyCombatItem& item : EnemyMeleeItems)
	{
		if (item.IsValid())
		{
			oldestAttackTime = std::min(oldestAttackTime, item.LastAttackTime);
		}
	}

	UpdateScoring(EnemyRangedItems, [this, playerLocation, oldestAttackTime](FRGX_EnemyCombatItem& item)
		{
			item.Distance = std::numeric_limits<float>::infinity();
			item.Scoring = std::numeric_limits<float>::infinity();

			if (item.IsValid())
			{
				item.Distance = FVector::Dist2D(playerLocation, item.Enemy->GetActorLocation());
				const float speed = item.Enemy->MoveSpeed;

				const float distanceScore = (item.Distance/speed) * DistanceWeight;
				const float visibilityScore = (item.Enemy->IsInFrustum() ? 0.0 : IsNotInFrustumScore) * IsNotInFrustumWeight;
				const float lastAttackTimeScore = (item.LastAttackTime-oldestAttackTime) * LastAttackTimeWeight;

				item.Scoring = distanceScore + visibilityScore + lastAttackTimeScore;
			}
		});



	UpdateSlots(EnemyRangedItems, NbRangedSlots);
}

void ARGX_CombatManager::UpdateSlots(TArray<FRGX_EnemyCombatItem>& EnemyItems, int32 numSlots)
{
	TArray<int32> candidates;
	int32 numAttackers = 0;
	int32 numRecoveries = 0;

	PrepareCandidateData(EnemyItems, candidates, numAttackers, numRecoveries);

	const float currentTime = UGameplayStatics::GetTimeSeconds(this);
	int32 numFreeSlots = numSlots - numAttackers;
	while (numFreeSlots > 0 && candidates.Num())
	{
		//const int32 index = FindNewAttacker(candidates, EnemyItems);
		const int32 candidateIndex = FindBestAttacker(candidates, EnemyItems);
		const int32 index = candidates[candidateIndex];
		auto& item = EnemyItems[index];

		// assert state holding
		ARGX_EnemyBaseController* enemyController = item.Enemy->GetController<ARGX_EnemyBaseController>();
		enemyController->SetEnemyAIState(ERGX_EnemyAIState::Attacking);
		item.LastAttackTime = currentTime;

		candidates.RemoveAtSwap(candidateIndex);
		//++numAttackers;
		--numFreeSlots;
	}
}

void ARGX_CombatManager::PrepareCandidateData(const TArray<FRGX_EnemyCombatItem>& EnemyItems, TArray<int32>& candidates, int32& numAttackers, int32& numRecoveries) const
{
	candidates.Empty();
	numAttackers = 0;
	numRecoveries = 0;

	const int32 lastIndex = EnemyItems.IndexOfByPredicate([](const FRGX_EnemyCombatItem& item) { return item.IsValid() == false; });
	for (int32 index = 0; index < lastIndex && index < NbHoldingEnemies; ++index)
	{
		const auto& item = EnemyItems[index];
		if (ARGX_EnemyBaseController* enemyController = item.Enemy->GetController<ARGX_EnemyBaseController>())
		{
			switch (enemyController->GetEnemyAIState())
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
}

int32 ARGX_CombatManager::FindBestAttacker(const TArray<int32>& candidates, const TArray<FRGX_EnemyCombatItem>& EnemyItems) const
{
	return 0;
}

int32 ARGX_CombatManager::FindNewAttacker(const TArray<int32>& candidates, const TArray<FRGX_EnemyCombatItem>& EnemyItems) const
{
	const int32 numCandidates = candidates.Num();

	if (numCandidates == 1)
	{
		return 0;
	}

	float totalScoring = 0.f;
	for (int32 index : candidates)
	{
		auto& item = EnemyItems[index];
		totalScoring += item.Scoring;
	}

	float totalSum = 0.0;
	TArray<float> candidatesWeights; candidatesWeights.Reserve(numCandidates);
	for (int32 index : candidates)
	{
		auto& item = EnemyItems[index];
		const float weight = totalScoring - item.Scoring;
		candidatesWeights.Add(weight + totalSum);
		totalSum += weight;
	}
	
	float randomNumber = FMath::RandRange(0.f, totalScoring * 2.f);
	for (int32 index = 0; index < (numCandidates-1); ++index)
	{
		if (randomNumber < candidatesWeights[index])
		{
			return index;
		}
	}

	return numCandidates - 1;
}

void ARGX_CombatManager::UpdateScoring(TArray<FRGX_EnemyCombatItem>& EnemyItems, const std::function<void(FRGX_EnemyCombatItem&)>& ScoringFunction)
{
	for (FRGX_EnemyCombatItem& item : EnemyItems)
	{
		if (item.IsValid())
		{
			ScoringFunction(item);
		}
	}

	EnemyItems.Sort([this](const FRGX_EnemyCombatItem& left, const FRGX_EnemyCombatItem& right)
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
}