#include "RGX_ManaAttributeSet.h"

void URGX_ManaAttributeSet::PreAttributeChange(const FGameplayAttribute & Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, GetMaxMana());
	}
}

void URGX_ManaAttributeSet::PreAttributeBaseChange(const FGameplayAttribute & Attribute, float& NewValue) const
 {
	Super::PreAttributeBaseChange(Attribute, NewValue);
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, GetMaxMana());
	}
}