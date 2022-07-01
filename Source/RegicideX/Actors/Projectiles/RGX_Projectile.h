// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GenericTeamAgentInterface.h"
#include "RGX_Projectile.generated.h"

class UGameplayEffect;
class UStaticMeshComponent;

UCLASS()
class REGICIDEX_API ARGX_Projectile : public AActor, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARGX_Projectile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ProjectileMesh = nullptr;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGenericTeamId ProjectileTeam;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	/** Assigns Team Agent to given TeamID */
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;

	/** Retrieve team identifier in form of FGenericTeamId */
	virtual FGenericTeamId GetGenericTeamId() const override;

public:
	/* Actor who fired the bullet */
	AActor* Instigator = nullptr;

protected:
	/* Effects the projectile applies when hitting a target */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayEffect>> EffectToApply;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BaseSpeed = 500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float LifeSpan = 10.0f;
};
