// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "RegicideX/Interfaces/RGX_GameplayTagInterface.h"
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

class UMCV_AbilitySystemComponent;
class URGX_HealthAttributeSet;

UCLASS()
class REGICIDEX_API ARGX_EnemyBase : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface, public IRGX_GameplayTagInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InterpSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRadius = 700.0f;

protected:
	/** Ability System Component to be used */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UMCV_AbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	URGX_HealthAttributeSet* HealthAttributeSet;

public:
	// Sets default values for this character's properties
	ARGX_EnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/** Movement methods */
	virtual void RotateToTarget(float DeltaTime);

	virtual void MoveToTarget(float DeltaTime, FVector TargetPos);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void HandleDamage(FAttackInfo info);

	virtual void HandleDeath();

	/** GameplayTagAssetInterface methods */
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;
	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;

	/** RX_GameplayTagInterface methods */
	virtual void AddGameplayTag(const FGameplayTag& TagToAdd) override;

	virtual void RemoveGameplayTag(const FGameplayTag& TagToRemove) override;

};
