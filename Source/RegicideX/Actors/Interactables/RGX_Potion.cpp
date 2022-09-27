// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/Actors/Interactables/RGX_Potion.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"

// Sets default values
ARGX_Potion::ARGX_Potion()
{
}

void ARGX_Potion::Interact_Implementation(AActor* ActorInteracting)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(ActorInteracting);
	if (TargetASC)
	{
		TargetASC->ApplyGameplayEffectToSelf(EffectToApply->GetDefaultObject<UGameplayEffect>(), 1.0f, TargetASC->MakeEffectContext());
	}

	Destroy();
}

void ARGX_Potion::BeginPlay()
{
	Super::BeginPlay();
	
}

