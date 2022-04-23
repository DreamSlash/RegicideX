#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTags.h"
#include "RGX_GameplayTagInterface.generated.h"

/** Interface for assets which contain gameplay tags */
UINTERFACE(Blueprintable, meta = (CannotImplementInterfaceInBlueprint))
class URGX_GameplayTagInterface : public UInterface
{
	GENERATED_BODY()
};

class REGICIDEX_API IRGX_GameplayTagInterface
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = RXGameplayTags)
	virtual void AddGameplayTag(const FGameplayTag& TagToAdd) = 0;

	UFUNCTION(BlueprintCallable, Category = RXGameplayTags)
	virtual void RemoveGameplayTag(const FGameplayTag& TagToRemove) = 0;
};