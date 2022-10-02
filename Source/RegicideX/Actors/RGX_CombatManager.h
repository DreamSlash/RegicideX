// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <functional>
#include "GameFramework/Actor.h"
#include "RGX_CombatManager.generated.h"

class ARGX_EnemyBase;
class ARGX_EnemyBaseController;
class ARGX_PlayerCharacter;

USTRUCT()
struct REGICIDEX_API FRGX_EnemyCombatItem
{
	GENERATED_BODY()

	FRGX_EnemyCombatItem() = default;
	FRGX_EnemyCombatItem(ARGX_EnemyBase* enemy)
		: Enemy(enemy)
	{}

	bool IsValid() const { return Enemy.IsValid(); }

	void Reset(ARGX_EnemyBase* enemy);

	UPROPERTY()
	TWeakObjectPtr<ARGX_EnemyBase> Enemy = nullptr;

	float Distance = 0.0;
	float Scoring = 0.0;
	float LastAttackTime = 0.0;
	
};

UCLASS()
class REGICIDEX_API ARGX_CombatManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARGX_CombatManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CombatRadius = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NbHoldingEnemies = 6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxMeleeEnemies = 12;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxRangedEnemies = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NbMeleeSlots = 2;

	UPROPERTY(Editanywhere, BlueprintReadWrite)
	int32 NbRangedSlots = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InvalidateOffset = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DistanceWeight = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float IsNotInFrustumScore = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float IsNotInFrustumWeight = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LastAttackTimeWeight = 1.0f;

	UPROPERTY()
	TArray<FRGX_EnemyCombatItem> EnemyMeleeItems;

	UPROPERTY()
	TArray<FRGX_EnemyCombatItem> EnemyRangedItems;

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

	void Tick(float DeltaTime) override;

private:
	UFUNCTION(BlueprintCallable)
	void Invalidate();

	UFUNCTION()
	void OnEnemyDeath(ARGX_EnemyBase* enemy);

private:
	void OnActorSpawned(AActor* actor);
	void OnEnemySpawned(ARGX_EnemyBase* Enemy, TArray<FRGX_EnemyCombatItem>& EnemyItems);

private:
	void InvalidateImpl(TArray<FRGX_EnemyCombatItem>& EnemyItems);

	void UpdateMeleeEnemies();
	void UpdateDistanceEnemies();

	void UpdateScoring(TArray<FRGX_EnemyCombatItem>& EnemyItems, const std::function<void(FRGX_EnemyCombatItem&)>& ScoringFunction);
	void UpdateSlots(TArray<FRGX_EnemyCombatItem>& EnemyItems, int32 numSlots);
	void PrepareCandidateData(const TArray<FRGX_EnemyCombatItem>& EnemyItems, TArray<int32>& candidates, int32& numAttackers, int32& numRecoveries) const;

	int32 FindBestAttacker(const TArray<int32>& candidates, const TArray<FRGX_EnemyCombatItem>& EnemyItems) const;
	int32 FindNewAttacker(const TArray<int32>& candidates, const TArray<FRGX_EnemyCombatItem>& EnemyItems) const;

private:
	UPROPERTY()
	TWeakObjectPtr<ARGX_PlayerCharacter> Player;

	/*TArray<int32> PeasantSlots;
	TArray<int32> AngelSlots;

	UPROPERTY()
	TArray<ARGX_EnemyBase*> EnemyList;

	UPROPERTY()
	TArray<ARGX_EnemyBase*> EnemiesIdling;

	UPROPERTY()
	TArray<ARGX_EnemyBase*> EnemiesWaiting;

	UPROPERTY()
	TArray<ARGX_EnemyBase*> EnemiesHolding;

	UPROPERTY()
	TMap<ARGX_EnemyBase*, float> EnemyScoring;*/

	/*UPROPERTY()
	TArray<ARGX_EnemyBase*> PeasantSlots;

	UPROPERTY()
	TArray<ARGX_EnemyBase*> AngelSlots;*/

	FVector LastPlayerPosition;

	bool bAddedNewEnemies = false;
	bool bEnemyDead = false;

};
