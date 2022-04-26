#include "RGX_HealthAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

void URGX_HealthAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, GetMaxHealth());
	}
}

void URGX_HealthAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	UE_LOG(LogTemp, Warning, TEXT("Health Pre attribute change"));

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, GetMaxHealth());
	}
}

void URGX_HealthAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
}