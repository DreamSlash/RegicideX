// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"
#include "RegicideX/Interfaces/RGX_InteractInterface.h"
#include "RGX_Potion.generated.h"

class URGX_AbilitySystemComponent;

UCLASS()
class REGICIDEX_API ARGX_Potion : public AActor, public IRGX_InteractInterface
{
	GENERATED_BODY()
	
public:	
	ARGX_Potion();

	void Interact_Implementation(AActor* ActorInteracting) override;
	void StartCanInteract_Implementation(AActor* ActorInteracting) override { /* Popup UI Element*/ }
	void StopCanInteract_Implementation(AActor* ActorInteracting) override { /* Hide UI Element*/ }

protected:
	void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Potion)
	TSubclassOf<UGameplayEffect> EffectToApply;
};
