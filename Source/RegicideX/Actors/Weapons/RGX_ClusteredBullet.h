// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GenericTeamAgentInterface.h"
#include "RGX_ClusteredBullet.generated.h"

class UStaticMeshComponent;
class URGX_HitboxComponent;

UCLASS()
class REGICIDEX_API ARGX_ClusteredBullet : public AActor, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARGX_ClusteredBullet();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* BulletMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		URGX_HitboxComponent* BulletCollider = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FGenericTeamId CharacterTeam;

	UPROPERTY(EditDefaultsOnly)
		USceneComponent* RootScene;

	UPROPERTY(EditDefaultsOnly)
		float speed = 10.0f;

private:
	FTimerHandle DestroyTimeHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void HandleDeath();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Assigns Team Agent to given TeamID */
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;

	/** Retrieve team identifier in form of FGenericTeamId */
	virtual FGenericTeamId GetGenericTeamId() const override;

};
