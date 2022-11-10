// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RGX_LoadLevelTrigger.generated.h"

UCLASS()
class REGICIDEX_API ARGX_LoadLevelTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARGX_LoadLevelTrigger();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* Box = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UChildActorComponent* Portal = nullptr;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnLevelsLoaded();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ARGX_Arena* Arena = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> LevelsToLoad;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> LevelsToUnload;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* TeleportLocation = nullptr;

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

private:
	UFUNCTION(BlueprintCallable)
	void OnArenaDeactivated();

	UFUNCTION(BlueprintCallable)
	void OnTeleportDone();

	UFUNCTION()
	void OnPortalOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnLevelShown();

private:
	int32 PendingLevelsToLoad = 0;

};
