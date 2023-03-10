#pragma once

#include "GameplayEffectTypes.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "RGX_GameplayEffectContext.generated.h"

USTRUCT(BlueprintType)
struct REGICIDEX_API FRGX_GameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_USTRUCT_BODY()

public:
	UScriptStruct* GetScriptStruct() const override
	{
		return FRGX_GameplayEffectContext::StaticStruct();
	}

	FRGX_GameplayEffectContext* Duplicate() const override
	{
		FRGX_GameplayEffectContext* NewContext = new FRGX_GameplayEffectContext();
		*NewContext = *this;
		NewContext->AddActors(Actors);
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}

		return NewContext;
	}

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

public:
	/* For damage/healing effects*/
	float DamageAmount;
	float ScalingAttributeFactor;

	/* For level up effect */
	float NewMaxHealth;
	float NewAttackPower;

	// Payload
	UObject* OptionalObject = nullptr;
};

template<>
struct TStructOpsTypeTraits<FRGX_GameplayEffectContext> : public TStructOpsTypeTraitsBase2<FRGX_GameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};