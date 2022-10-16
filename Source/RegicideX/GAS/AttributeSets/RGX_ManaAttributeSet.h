#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "RGX_ManaAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
       GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
       GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
       GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
       GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
 class REGICIDEX_API URGX_ManaAttributeSet : public UAttributeSet
 {
	GENERATED_BODY()
		
public:
	
	virtual void PreAttributeChange(const FGameplayAttribute & Attribute, float& NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute & Attribute, float& NewValue) const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Mana = 0.0f;
	ATTRIBUTE_ACCESSORS(URGX_ManaAttributeSet, Mana)
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData MaxMana = 100.0f;
	ATTRIBUTE_ACCESSORS(URGX_ManaAttributeSet, MaxMana)
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData ManaStack = 0.0f;
	ATTRIBUTE_ACCESSORS(URGX_ManaAttributeSet, ManaStack)
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData MaxManaStack = 6.0f;
	ATTRIBUTE_ACCESSORS(URGX_ManaAttributeSet, MaxManaStack)
};