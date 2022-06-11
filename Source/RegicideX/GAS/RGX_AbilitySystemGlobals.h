#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "RGX_AbilitySystemGlobals.generated.h"

UCLASS()
class REGICIDEX_API URGX_AbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

public:
	URGX_AbilitySystemGlobals();

	static URGX_AbilitySystemGlobals& RGXGet()
	{
		return dynamic_cast<URGX_AbilitySystemGlobals&>(Get());
	}

	FGameplayEffectContext* AllocGameplayEffectContext() const override;
};