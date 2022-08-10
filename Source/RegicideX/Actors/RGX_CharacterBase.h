// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "RegicideX/GAS/AttributeSets/RGX_AttributeSet.h"
#include "RegicideX/GAS/GameplayAbilities/BaseAbilities/RGX_GameplayAbility.h"
#include "RegicideX/GAS/RGX_PayloadObjects.h"
#include "RGX_CharacterBase.generated.h"

class UMCV_AbilitySystemComponent; // TODO Change to RGX
class URGX_GameplayAbility;
class AActor;

/** Base Class for al characters in the game */
UCLASS()
class REGICIDEX_API ARGX_CharacterBase : public ACharacter, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:

	// Constructor and overrides
	ARGX_CharacterBase();
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/** Returns current health, will be 0 if dead */
	UFUNCTION(BlueprintCallable)
	virtual float GetHealth() const;

	/** Returns maximum health, health will never be greater than this */
	UFUNCTION(BlueprintCallable)
	virtual float GetMaxHealth() const;

	/** Returns the character level that is passed to the ability system */
	UFUNCTION(BlueprintCallable)
	virtual int32 GetCharacterLevel() const;

	/** Modifies the character level, this may change abilities. Returns true on success */
	UFUNCTION(BlueprintCallable)
	virtual bool SetCharacterLevel(int32 NewLevel);

	UFUNCTION(BlueprintCallable)
	bool IsAlive();

	UFUNCTION(BlueprintCallable)
	void OnBeingLaunched(
		AActor* ActorInstigator, 
		URGX_LaunchEventDataAsset* LaunchPayload,
		float LaunchDelay = 0.2f);

protected:
	/** The level of this character, should not be modified directly once it has already spawned */
	UPROPERTY(EditAnywhere, Category = Abilities)
	int32 CharacterLevel;

	/** Indicates the character team. For enemies should be 1. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	FGenericTeamId CharacterTeam;

	/** Abilities to grant to this character on creation. These will be activated by tag or event and are not bound to specific inputs */
	UPROPERTY(EditDefaultsOnly, Category = Abilities)
	TArray<FRGX_AbilityInitData> GameplayAbilities;

	/** Passive Gameplay Effects applied on creation. */
	UPROPERTY(EditDefaultsOnly, Category = Abilities)
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	/** Gameplay Ability System components */
	UPROPERTY()
	UMCV_AbilitySystemComponent* AbilitySystemComponent = nullptr;

	/** Attributes forming and modifying the Ability System */
	UPROPERTY()
	URGX_AttributeSet* AttributeSet = nullptr;

	/** Check if abilities are initialized. */
	UPROPERTY()
	bool bAbilitiesInitialized = false;

	/** Events called from attribute set changes to decouple the logic. They call BP events. */
	virtual void HandleDamage(
		float DamageAmount,
		const FHitResult& HitInfo,
		const struct FGameplayTagContainer& DamageTags,
		ARGX_CharacterBase* InstigatorCharacter,
		AActor* DamageCauser);

	virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	UFUNCTION(BlueprintCallable)
	virtual void HandleDeath();

	/** Called when character takes damage and may be killed. */
	UFUNCTION(BlueprintImplementableEvent)
	void OnDamaged(
		float DamageAmount, 
		const FHitResult& HitInfo, 
		const struct FGameplayTagContainer& DamageTags, 
		ARGX_CharacterBase* InstigatorCharacter, 
		AActor* DamageCauser);

	/** Event called when health is changed, either by being damaged or restoring health. */
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHandleDeath();

	/** Apply the startup gameplay abilities and effects */
	void AddStartupGameplayAbilities();

	/** Attempts to remove any startup gameplay abilities */
	void RemoveStartupGameplayAbilities();

	virtual FGenericTeamId GetGenericTeamId() const override;

	friend URGX_AttributeSet;
};
