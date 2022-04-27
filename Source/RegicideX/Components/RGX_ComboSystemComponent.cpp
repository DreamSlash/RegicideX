#include "RGX_ComboSystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"

URGX_ComboSystemComponent::URGX_ComboSystemComponent()
{

}

void URGX_ComboSystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URGX_ComboSystemComponent::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void URGX_ComboSystemComponent::ManageInputToken(ERGXPlayerInputID PlayerInput)
{
}
