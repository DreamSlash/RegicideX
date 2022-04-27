#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTags.h"
#include "../Enums/RGX_InputEnums.h"
#include "RGX_ComboSystemComponent.generated.h"

USTRUCT()
struct FRGX_ComboTransition
{
	GENERATED_BODY()

	// Possible inputs to continue with the combo
	UPROPERTY(EditAnywhere)
	TArray<ERGXPlayerInputID> NextPotentialInputs;

	// Possible attack transitions
	UPROPERTY(EditAnywhere)
	TArray<FGameplayTag> NextPotentialAttacks;
};

UCLASS(meta = (BlueprintSpawnableComponent))
class REGICIDEX_API URGX_ComboSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URGX_ComboSystemComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	void ManageInputToken(ERGXPlayerInputID PlayerInput);

protected:
	UPROPERTY(EditDefaultsOnly, Category = Combo)
	TMap<FGameplayTag, FRGX_ComboTransition> ComboMap;
};