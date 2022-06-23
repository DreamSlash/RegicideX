#pragma once

#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "RGX_AttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/** Holds all of the attributes used by abilities. */
UCLASS()
class REGICIDEX_API URGX_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:

	URGX_AttributeSet();
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	/** This is the Character health. Capped by the MaxHealth and expected character to die when reached 0. */
	UPROPERTY(EditAnywhere, Category = "Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(URGX_AttributeSet, Health)

	/** Maximum Health for the character. Can scale depending on level. */
	UPROPERTY(EditAnywhere, Category = "Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(URGX_AttributeSet, MaxHealth)

	/** AttackPower of the attacker is multiplied by the base Damage to reduce health, so 1.0 means no bonus */
	UPROPERTY(EditAnywhere, Category = "Damage")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(URGX_AttributeSet, AttackPower)

	/** Damage is a 'temporary' attribute used by the DamageExecution to calculate final damage, which then turns into -Health */
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(URGX_AttributeSet, Damage)

protected:
	/** Function to proportionally increase the attribute value when its Max attribute is increased in game (For a level update) and maintains the proportional part of the increase. */
	void AdjustAttributeForMaxChange(
		FGameplayAttributeData& AffectedAttribute, 
		const FGameplayAttributeData& MaxAttribute, 
		float NewMaxValue, 
		const FGameplayAttribute& AffectedAttributeProperty);
};