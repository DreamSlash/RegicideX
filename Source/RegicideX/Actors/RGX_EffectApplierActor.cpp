// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_EffectApplierActor.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"
#include "GameplayTags.h"
#include "RegicideX/GAS/RGX_GameplayEffectContext.h"

// Sets default values
ARGX_EffectApplierActor::ARGX_EffectApplierActor()
	: AActor()
{

}

void ARGX_EffectApplierActor::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	ActorTeam = TeamID;
}

FGenericTeamId ARGX_EffectApplierActor::GetGenericTeamId() const
{
	return ActorTeam;
}

bool ARGX_EffectApplierActor::OnPlayerOverlaps(AActor* Player)
{
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Player, true))
	{
		if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("PossessedBy.Player")))
		{
			for (const FRGX_EffectContextContainer& effectContextContainer : EffectToApplyToTargetWithPayload)
			{
				FGameplayEffectContextHandle effectContext = ASC->MakeEffectContext();
				FRGX_GameplayEffectContext* gameplayEffectContext = static_cast<FRGX_GameplayEffectContext*>(effectContext.Get());
				gameplayEffectContext->OptionalObject = effectContextContainer.Payload;

				const UGameplayEffect* gameplayEffect = effectContextContainer.EffectToApply->GetDefaultObject<UGameplayEffect>();
				constexpr int32 level = 0;
				ASC->ApplyGameplayEffectToSelf(gameplayEffect, level, effectContext);
			}

			return true;
		}
	}

	return false;
}
