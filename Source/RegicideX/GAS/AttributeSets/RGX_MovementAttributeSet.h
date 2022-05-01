#pragma once

#include "CoreMinimal.h"
#include "AttributeSets/MCV_AttributeSet.h"
#include "RGX_MovementAttributeSet.generated.h"

/**
 * Handles movement attributres of our characters
 */
UCLASS()
class REGICIDEX_API URGX_MovementAttributeSet : public UMCV_AttributeSet
{
	GENERATED_BODY()

public:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData MovementSpeed = 600.0f;
	ATTRIBUTE_ACCESSORS(URGX_MovementAttributeSet, MovementSpeed)

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData MinMovementSpeed = 300.0f;
	ATTRIBUTE_ACCESSORS(URGX_MovementAttributeSet, MinMovementSpeed)

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData MaxMovementSpeed = 900.0f;
	ATTRIBUTE_ACCESSORS(URGX_MovementAttributeSet, MaxMovementSpeed)

};
