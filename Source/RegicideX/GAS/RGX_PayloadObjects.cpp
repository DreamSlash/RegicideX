#include "RGX_PayloadObjects.h"

URGX_LaunchEventPayload::URGX_LaunchEventPayload()
{

}

URGX_DamageEventDataAsset::URGX_DamageEventDataAsset()
{
	EventTag = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Effect.Damage"));
}

URGX_LaunchEventDataAsset::URGX_LaunchEventDataAsset()
{
	EventTag = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Effect.Launch"));
}