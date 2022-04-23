// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSets/MCV_AttributeSet.h"
#include "RGX_HealthAttributeSet.generated.h"

/**
 * 
 */


UCLASS()
class REGICIDEX_API URGX_HealthAttributeSet : public UMCV_AttributeSet
{
	GENERATED_BODY()

public:

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const  override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Health = 100.0f;
	ATTRIBUTE_ACCESSORS(URGX_HealthAttributeSet, Health)

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData MaxHealth = 100.0f;
	ATTRIBUTE_ACCESSORS(URGX_HealthAttributeSet, MaxHealth)
};
