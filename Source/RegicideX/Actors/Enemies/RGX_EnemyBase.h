// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffect.h"
#include "RegicideX/Actors/RGX_PoolActor.h"
#include "RegicideX/Interfaces/RGX_GameplayTagInterface.h"
#include "RegicideX/Interfaces/RGX_InteractInterface.h"
#include "RGX_EnemyBase.generated.h"

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

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHandleDeath, int)

/* Struct to inform about when the attack was received*/
UCLASS()
class REGICIDEX_API ARGX_EnemyBase : public ARGX_PoolActor, public IGameplayTagAssetInterface, public IRGX_GameplayTagInterface, public IRGX_InteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARGX_EnemyBase();

	virtual void Activate() override;
	virtual void Deactivate() override;

public:
	
	FOnHandleDeath OnHandleDeathEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InterpSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRadius = 700.0f;

	/** The base score all enemies will give to the player when they die. Each class should change its value accordingly. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ScoreValue = 10;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	URGX_HitboxesManagerComponent* HitboxesManager = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanBeKnockup = true;

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

protected:

	// Called when the game starts or when spawned
	void BeginPlay() override;

	void PossessedBy(AController* NewController) override;

	UFUNCTION()
	void EraseRecentDamage(const float DamageAmount);

	// FGenericTeamId interface
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	// End of FGenericTeamId interface

public:
	virtual FGenericTeamId GetGenericTeamId() const override;

	/** Movement methods */
	virtual void RotateToTarget(float DeltaTime);

	virtual void MoveToTarget(float DeltaTime, FVector TargetPos);
	// ---------------------

	void EnableInteraction();
	void DisableInteraction();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; 

	//virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void HandleDamage(FAttackInfo info); //TODO: what is this

	/** Events called from attribute set changes to decouple the logic. They call BP events. */
	//virtual void HandleDamage(float DamageAmount, AActor* DamageCauser);
	//virtual void HandleHealthChanged(float DeltaValue);
	//virtual void HandleDeath();

	virtual void HandleDamage(
		float DamageAmount,
		const FHitResult& HitInfo,
		const struct FGameplayTagContainer& DamageTags,
		ARGX_CharacterBase* InstigatorCharacter,
		AActor* DamageCauser) override;

	virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags) override;
	virtual void HandleDeath() override;

	///* BP events */
	//UFUNCTION(BlueprintImplementableEvent)
	//void OnHandleDamage(float DamageAmount, AActor* DamageCauser);
	//UFUNCTION(BlueprintImplementableEvent)
	//void OnHandleHealthChanged(float DeltaValue);
	//UFUNCTION(BlueprintImplementableEvent)
	//void OnHandleDeath();

	/** GameplayTagAssetInterface methods */
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;
	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;

	/** RX_GameplayTagInterface methods */
	virtual void AddGameplayTag(const FGameplayTag& TagToAdd) override;

	virtual void RemoveGameplayTag(const FGameplayTag& TagToRemove) override;

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
};
