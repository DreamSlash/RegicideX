// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffect.h"
#include "RegicideX/Actors/RGX_PoolActor.h"
#include "RGX_EnemyBase.generated.h"

UENUM(BlueprintType)
enum class ERGX_EnemyType : uint8
{
	None				UMETA(DisplayName = "None"),
	MeleeAngel			UMETA(DisplayName = "MeleeAngel"),
	DistanceAngel		UMETA(DisplayName = "DistanceAngel"),
	MageAngel			UMETA(DisplayName = "MageAngel"),
	MeleePeasant		UMETA(DisplayName = "MeleePeasant"),
	ShieldPeasant		UMETA(DisplayName = "ShieldPeasant"),
	DistancePeasant		UMETA(DisplayName = "DistancePeasant"),
	SuicidalPeasant		UMETA(DisplayName = "SuicidalPeasant")
};

UENUM(BlueprintType)
enum ERGX_EnemyAIState
{
	None		UMETA(DisplayName = "None"),
	Attacking	UMETA(DisplayName = "Attacking"),
	Holding		UMETA(DisplayName = "Holding"),
	Waiting		UMETA(DisplayName = "Waiting"),
	Recovering	UMETA(DisplayName = "Recovering")
};

USTRUCT()
struct FAttackInfo {

	GENERATED_BODY()

	UPROPERTY()
	float BaseDamage;

	UPROPERTY()
	bool Launch;

	UPROPERTY()
	float DamageMultiplier;

	UPROPERTY()
	FVector DamageOrigin;

	UPROPERTY()
	FVector LaunchVector;

};

class USphereComponent;
class UWidgetComponent;
class URGX_HitboxesManagerComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHandleDeathSignature, class ARGX_EnemyBase*, EnemyKilled);

/* Struct to inform about when the attack was received*/
UCLASS(BlueprintType)
class REGICIDEX_API ARGX_EnemyBase : public ARGX_PoolActor, public IRGX_InteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARGX_EnemyBase();

	virtual void Activate() override;
	virtual void Deactivate() override;

	UFUNCTION(BlueprintCallable)
	ERGX_EnemyType GetEnemyType() const;

	UFUNCTION(BlueprintCallable)
	ERGX_EnemyAIState GetEnemyAIState() const;
	UFUNCTION(BlueprintCallable)
	void SetEnemyAIState(ERGX_EnemyAIState state);

public:
	
	FOnHandleDeathSignature OnHandleDeathEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotationInterpSpeed = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRotationInterpSpeed = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OrbitSpeed = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRadius = 700.0f;

	/** The base score all enemies will give to the player when they die. Each class should change its value accordingly. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ScoreValue = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Orbiting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	URGX_HitboxesManagerComponent* HitboxesManager = nullptr;

protected:

	/** Target widget component to notify the player this is the enemy on target. */
	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* CombatTargetWidgetComponent = nullptr;

	/** Health Display Widget */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* HealthDisplayWidgetComponent = nullptr;

	/* Percentage of health player must apply as recent damage to weaken enemy */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	float WeakenPercentage = 0.3f;

	/** Collider used for interacting with this actor */
	UPROPERTY(EditAnywhere)
	USphereComponent* InteractionShapeComponent = nullptr;

	// Debug
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UWidgetComponent* DebugAttributesWidgetComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float HealthBarHideDistance = 800.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RecentDamageSeconds = 2.0f;

	float RecentDamage;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> SoulParticleActor = nullptr;

	UPROPERTY(EditDefaultsOnly)
	ERGX_EnemyType EnemyType;

	// The cosinus where the enemy do not move to player while rotating but instead, it turns in place after a while
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TurnInPlaceCos = -0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TimeBeforeTurningInPlace = 2.0f;
	float TimeLostSight;

	// Stop turning in place and move to player when the enemy's forward has this cos after having lost sight with player
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RegainSightCos = 0.3f;

	// This is true when the TurnInPlaceCos is reached, but turn in place do not reset (waiting the TimeBeforeTurningInPlace)
	// until the enemy has gained sight again
	bool bHasLostSightOfPlayer = false;

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;
	void PossessedBy(AController* NewController) override;

	UFUNCTION()
	void EraseRecentDamage(const float DamageAmount);

	void CheckIfWeak(float DamageAmount);
	void CheckIfHasLostSightOfPlayer();

	// FGenericTeamId interface
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	// End of FGenericTeamId interface

	void SpawnSouls(const int Quantity);

public:
	virtual FGenericTeamId GetGenericTeamId() const override;

	/** Movement methods */
	UFUNCTION(BlueprintCallable)
	virtual void RotateToTarget(float DeltaTime);

	virtual void MoveToTarget(float DeltaTime, FVector TargetPos);
	// ---------------------

	UFUNCTION(BlueprintCallable)
	void StopLogic(const FString& Reason);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bWeak = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDefaultFocusPlayer = false;

	UFUNCTION(BlueprintCallable)
	bool IsWeak();

	void EnableInteraction();

	UFUNCTION(BlueprintCallable)
	void DisableInteraction();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Events called from attribute set changes to decouple the logic. They call BP events. */
	virtual void HandleDamage(
		float DamageAmount,
		const FHitResult& HitInfo,
		const struct FGameplayTagContainer& DamageTags,
		ARGX_CharacterBase* InstigatorCharacter,
		AActor* DamageCauser,
		ERGX_AnimEvent HitReactFlag) override;

	virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags) override;
	virtual void HandleDeath() override;

	/** Combat assist widget functions */
	void ShowCombatTargetWidget();
	void HideCombatTargetWidget();
	// ----------------------------------

	/** Interact Interface */
	void Interact(AActor* ActorInteracting) override;
	void StartCanInteract(AActor* ActorInteracting) override;
	void StopCanInteract(AActor* ActorInteracting) override;
	bool CanBeInteractedWith(AActor* ActorInteracting) override;

	bool IsInFrustum();

	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	ERGX_EnemyAIState AIState = ERGX_EnemyAIState::None;

};
