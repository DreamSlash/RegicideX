// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_BlueprintLibrary.h"
#include "RegicideX/GAS/RGX_GameplayEffectContext.h"

void URGX_BlueprintLibrary::ApplyGameplayEffectContextContainerToActor(const ARGX_CharacterBase* CharacterBase, TSubclassOf<UGameplayEffect> EffectToApply, URGX_RGXEventDataAsset* Payload)
{
	if (EffectToApply.Get() == nullptr) return;

	UAbilitySystemComponent* ACS = CharacterBase->GetAbilitySystemComponent();
	if (ACS)
	{
		FGameplayEffectContextHandle EffectContextHandle = ACS->MakeEffectContext();
		FGameplayEffectSpecHandle GameplayEffectSpecHandle = ACS->MakeOutgoingSpec(EffectToApply, CharacterBase->GetCharacterLevel(), EffectContextHandle);

		FRGX_GameplayEffectContext* RGXContextHandle = static_cast<FRGX_GameplayEffectContext*>(EffectContextHandle.Get());
		RGXContextHandle->OptionalObject = Payload;

		FGameplayEffectSpec* GESpec = GameplayEffectSpecHandle.Data.Get();
		ACS->ApplyGameplayEffectSpecToSelf(*GESpec);
	}
}
