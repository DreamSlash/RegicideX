// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
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


UCLASS()
class REGICIDEX_API ARGX_EnemyBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Ability System Component to be used */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UMCV_AbilitySystemComponent* AbilitySystemComponent;

public:
	// Sets default values for this character's properties
	ARGX_EnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void HandleDamage(FAttackInfo info);

	virtual void HandleDeath();

};
