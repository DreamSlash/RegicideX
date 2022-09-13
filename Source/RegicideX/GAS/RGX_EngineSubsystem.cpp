#include "RGX_EngineSubsystem.h"
#include "AbilitySystemGlobals.h"

void URGX_EngineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UAbilitySystemGlobals::Get().InitGlobalData();
}
