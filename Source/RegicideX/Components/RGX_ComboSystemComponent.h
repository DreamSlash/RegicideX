#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTags.h"
#include "RegicideX/Enums/RGX_InputEnums.h"
#include "RGX_ComboSystemComponent.generated.h"

USTRUCT()
struct FRGX_ComboTransition
{
	GENERATED_BODY()

	// Possible inputs to continue with the combo
	UPROPERTY(EditAnywhere)
	TArray<ERGX_ComboTokenID> NextPotentialInputs;

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

	void BeginPlay() override;
	void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	FGameplayTag ManageInputToken(ERGX_ComboTokenID PlayerInput, bool bIsOnAir, bool bCanAirCombo);
	FGameplayTag GetNextAttack();
	void CleanStatus(int32 ActivatedAbilities);

protected:

	UPROPERTY(EditDefaultsOnly, Category = Combo)
	TMap<FGameplayTag, FRGX_ComboTransition> ComboMap;

protected:
	// Combo managing variables
	UPROPERTY()
	FGameplayTag CurrentAttack = FGameplayTag::RequestGameplayTag("Combo.None");

	UPROPERTY()
	FGameplayTag NextAttack = FGameplayTag::RequestGameplayTag("Combo.None");

	UPROPERTY()
	ERGX_ComboTokenID NextComboInput;

	UPROPERTY()
	bool bEnableComboFlag = false;

	UPROPERTY()
	bool bComboFlag = false;

	UPROPERTY()
	bool bAirComboFlag = false;
	// ------------------------

protected:

	UFUNCTION()
	void SetNextComboAttack(ERGX_ComboTokenID PlayerInput);

	UFUNCTION()
	void InitiateCombo(ERGX_ComboTokenID PlayerInput, bool bIsOnAir);

	UFUNCTION()
	FGameplayTag FindNextAttack(ERGX_ComboTokenID PlayerInput);

public:

	UFUNCTION(BlueprintCallable, Category = "Combo", Meta = (DisplayName = "IsAttacking"))
	bool IsAttacking();

	UFUNCTION(BlueprintCallable, Category = "Combo", Meta = (DisplayName = "OnCombo"))
	void OnCombo();

	UFUNCTION(BlueprintCallable, Category = "Combo", Meta = (DisplayName = "OnEnableCombo"))
	void OnEnableCombo();

	UFUNCTION(BlueprintCallable, Category = "Combo", Meta = (DisplayName = "OnDisableCombo"))
	void OnDisableCombo();

	UFUNCTION(BlueprintCallable, Category = "Combo", Meta = (DisplayName = "OnEndCombo"))
	void OnEndCombo();

	UFUNCTION()
	void DrawDebugInfo();

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetCurrentAttackTag() const;

};