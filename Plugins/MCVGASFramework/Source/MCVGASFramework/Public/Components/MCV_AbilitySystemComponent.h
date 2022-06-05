// Copyright 2022 David (Quod) Soler

#pragma once

#include "AbilitySystemComponent.h"
#include "Delegates/DelegateCombinations.h"
#include "GameFramework/Actor.h"
#include "MCV_AbilitySystemComponent.generated.h"

class UAbilityAnimNotify;
class USkeletalMeshComponent;
class UAnimSequenceBase;
class UMCV_AttributeSet;
class UDataTable;
class APawn;
class AController;

UENUM(BlueprintType)
enum class EMCVAbilityInputID : uint8
{
	None				UMETA(DisplayName = "None"),
	Confirm				UMETA(DisplayName = "Confirm"),
	Cancel				UMETA(DisplayName = "Cancel"),
	LightAttack			UMETA(DisplayName = "LightAttack"),
	CancelLightAttack	UMETA(DisplayName = "CancelLightAttack"),
	HeavyAttack			UMETA(DisplayName = "HeavyAttack"),
	CancelHeavyAttack	UMETA(DisplayName = "CancelHeavyAttack"),
	Evasion				UMETA(DisplayName = "Evasion"),
	CancelEvasion		UMETA(DisplayName = "CancelEvasion"),
	PowerSkill			UMETA(DisplayName = "PowerSkill"),
	CancelPowerSkill	UMETA(DisplayName = "CancelPowerSkill"),
	Ability2			UMETA(DisplayName = "Ability2"),
	Sprint				UMETA(DisplayName = "Sprint"),
	Jump				UMETA(DisplayName = "Jump"),
	StopJump			UMETA(DisplayName = "StopJump")
};

// Allows the creation of attribute sets and initialization with default values
USTRUCT()
struct FmcvAttributeApplication
{
	GENERATED_BODY()

	// Ability set to grant
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMCV_AttributeSet> AttributeSetType;

	// Data table reference to initialize the attributes with, if any (can be left unset)
	UPROPERTY(EditAnywhere, meta = (RequiredAssetDataTags = "RowStructure=AttributeMetaData"))
	UDataTable* InitializationData = nullptr;
};

// Data related to a single ability granting
USTRUCT()
struct FMCVAbilityInitData
{
	GENERATED_BODY()

	// Ability to grant
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> Ability;
	UPROPERTY(EditDefaultsOnly)
	EMCVAbilityInputID InputID = EMCVAbilityInputID::None;
};

/**
 *	Extended Ability System with extra features	such as granting default
 *  abilities and attributes.
 */
UCLASS(meta=(BlueprintSpawnableComponent))
class MCVGASFRAMEWORK_API UMCV_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	
	//~ Begin UAbilitySystemComponent interface
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
	//~ End UAbilitySystemComponent interface

	//~ Begin UObject interface
	virtual void BeginDestroy() override;
	//~ End UObject interface

	UFUNCTION(BlueprintCallable, Category = "AncientGame|Abilities")
	FGameplayAbilitySpecHandle GrantAbilityOfType(TSubclassOf<UGameplayAbility> AbilityType, bool bRemoveAfterActivation);

    // Exposes AddLooseGameplayTag to Blueprint. This tag is *not* replicated.
    UFUNCTION(BlueprintCallable, Category = "Abilities", Meta = (DisplayName = "AddLooseGameplayTag"))
    void K2_AddLooseGameplayTag(const FGameplayTag& GameplayTag, int32 Count = 1);

    // Exposes AddLooseGameplayTags to Blueprint. These tags are *not* replicated.
    UFUNCTION(BlueprintCallable, Category = "Abilities", Meta = (DisplayName = "AddLooseGameplayTags"))
    void K2_AddLooseGameplayTags(const FGameplayTagContainer& GameplayTags, int32 Count = 1);

    // Exposes RemoveLooseGameplayTag to Blueprint. This tag is *not* replicated.
    UFUNCTION(BlueprintCallable, Category = "Abilities", Meta = (DisplayName = "RemoveLooseGameplayTag"))
    void K2_RemoveLooseGameplayTag(const FGameplayTag& GameplayTag, int32 Count = 1);

    // Exposes RemoveLooseGameplayTags to Blueprint. These tags are *not* replicated.
    UFUNCTION(BlueprintCallable, Category = "Abilities", Meta = (DisplayName = "RemoveLooseGameplayTags"))
    void K2_RemoveLooseGameplayTags(const FGameplayTagContainer& GameplayTags, int32 Count = 1);

protected:
	// Grant default abilities and attributes
	void GrantDefaultAbilitiesAndAttributes();
	UFUNCTION() // UFunction to be able to bind with dynamic delegate
	void OnPawnControllerChanged(APawn* Pawn, AController* NewController);

public:
	// Default abilities to grant
	UPROPERTY(EditDefaultsOnly, Category = Ability)
	TArray<FMCVAbilityInitData> DefaultAbilities;
	// Default attributes to create
	UPROPERTY(EditDefaultsOnly, Category = Ability)
	TArray<FmcvAttributeApplication> DefaultAttributes;


protected:
	// List of default granted abilities
	TArray<FGameplayAbilitySpecHandle> DefaultAbilityHandles;
	// List of added attributes
	UPROPERTY(transient)
	TArray<UAttributeSet*> AddedAttributes;

public:
	bool bIsInitialized = false;
};
