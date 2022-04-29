// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
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
class REGICIDEX_API ARGX_EnemyBase : public ACharacter, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

	/** Ability System Component to be used */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UMCV_AbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	URGX_HealthAttributeSet* HealthAttributeSet;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGenericTeamId CharacterTeam;

public:
	// Sets default values for this character's properties
	ARGX_EnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// FGenericTeamId interface
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	// End of FGenericTeamId interface

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void HandleDamage(FAttackInfo info);

	virtual void HandleDeath();

};
