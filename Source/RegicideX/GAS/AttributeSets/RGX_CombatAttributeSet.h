#pragma once

#include "CoreMinimal.h"
#include "AttributeSets/MCV_AttributeSet.h"
#include "RGX_CombatAttributeSet.generated.h"

/**
 * Handles resistances and defenses
 */
UCLASS()
class REGICIDEX_API URGX_CombatAttributeSet : public UMCV_AttributeSet
{
	GENERATED_BODY()
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData AttackPower = 10.0f;
	ATTRIBUTE_ACCESSORS(URGX_CombatAttributeSet, AttackPower)

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData DamageMitigation = 0.0f;
	ATTRIBUTE_ACCESSORS(URGX_CombatAttributeSet, DamageMitigation)
};
