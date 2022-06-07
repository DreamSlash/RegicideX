#include "RGX_AbilitySystemGlobals.h"
#include "RegicideX/GAS/RGX_GameplayEffectContext.h"

URGX_AbilitySystemGlobals::URGX_AbilitySystemGlobals()
{

}

FGameplayEffectContext* URGX_AbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FRGX_GameplayEffectContext();
}