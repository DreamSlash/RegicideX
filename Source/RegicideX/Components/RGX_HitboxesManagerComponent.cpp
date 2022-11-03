#include "RGX_HitboxesManagerComponent.h"
#include "RGX_HitboxComponent.h"

URGX_HitboxesManagerComponent::URGX_HitboxesManagerComponent()
{

}

void URGX_HitboxesManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URGX_HitboxesManagerComponent::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void URGX_HitboxesManagerComponent::AddHitboxToMap(FGameplayTag IdentifierTag, URGX_HitboxComponent* HitboxComponent)
{
	HitboxesMap.Add(IdentifierTag, HitboxComponent);
}

URGX_HitboxComponent* URGX_HitboxesManagerComponent::GetHitboxByTag(FGameplayTag IdentifierTag)
{
	URGX_HitboxComponent* ComponentToReturn = nullptr;

	URGX_HitboxComponent** FoundComponent = HitboxesMap.Find(IdentifierTag);
	if (FoundComponent)
	{
		ComponentToReturn = *FoundComponent;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Get Hitbox by tag: %s\n"), *IdentifierTag.ToString());

	return ComponentToReturn;
}
