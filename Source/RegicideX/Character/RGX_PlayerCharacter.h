#pragma once

#include "CoreMinimal.h"
#include "Components/MCV_AbilitySystemComponent.h"
#include "GameplayTags.h"
#include "RegicideX/Actors/RGX_CharacterBase.h"
#include "RegicideX/Components/RGX_CombatAssistComponent.h"
#include "RegicideX/Enums/RGX_InputEnums.h"
#include "RGX_PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class URGX_AbilitySystemComponent;
class URGX_ANS_JumpComboSection;
class URGX_ComboSystemComponent;
class URGX_CombatAssistComponent;
class URGX_InputHandlerComponent;
class URGX_MovementAttributeSet;
class URGX_InteractComponent;
class URGX_LaunchEventDataAsset;
class UGameplayEffect;
class UWidgetComponent;

USTRUCT()
struct FRGX_LeanInfo
{
	GENERATED_BODY()

public:
	UPROPERTY()
	float LeanAmount;

	UPROPERTY(EditAnywhere)
	float InterSpeed;
};

UCLASS(config = Game)
class REGICIDEX_API ARGX_PlayerCharacter : public ARGX_CharacterBase
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera = nullptr;

	/** Combo System Component to manage player combos */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combos, meta = (AllowPrivateAccess = "true"))
	URGX_ComboSystemComponent* ComboSystemComponent = nullptr;

	/** Combat Assist Component to manage player movement while doing combat actions */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	URGX_CombatAssistComponent* CombatAssistComponent = nullptr;
	
	/** Input Handler component to manage player inputs */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	URGX_InputHandlerComponent* InputHandlerComponent = nullptr;

	/** Interact Component to manage actions with interactable actors in the world */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	URGX_InteractComponent* InteractComponent = nullptr;

	/** Interact Widget Component*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* InteractWidgetComponent = nullptr;

	/** Check if player is attacking, meaning the player has an active ability with Ability.Melee tag on it. */
	bool IsAttacking();

	// Attributes ---------------
	UPROPERTY()
	URGX_MovementAttributeSet* MovementAttributeSet = nullptr;

	// --------------------------
public:
	ARGX_PlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	// TODO [REFACTOR]: Move this to AbilitySystemComponent.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FGameplayTag> PowerSkills;

	UPROPERTY()
	uint8 CurrentSkillSelected = 0;

	UPROPERTY()
	FGameplayTag CurrentSkillTag;

	/** If ture, it is in window to keep on with the current combo. */
	UPROPERTY()
	bool bCanCombo = false;

	/** Holds the AnimNotifyState of the current attack, which has the information for the combo to follow. */
	UPROPERTY()
	URGX_ANS_JumpComboSection* JumpComboNotifyState;

	//--------------------------

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EObjectTypeQuery> DodgeableObjectType;

	UPROPERTY(EditDefaultsOnly)
	float DefaultGravity = 3.0f;

	UPROPERTY(EditDefaultsOnly)
	float MaxWalkSpeed = 600.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsFallingDown = false;

	void BeginPlay() override;

	void Tick(float DeltaTime) override;

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintNativeEvent)
	void OnJump();

	UFUNCTION(BlueprintCallable)
	void DisableMovementInput();

	UFUNCTION(BlueprintCallable)
	void EnableMovementInput();

	UFUNCTION(BlueprintCallable)
	void OnFollowCombo();

protected:
	/** Animation variables */
	UPROPERTY(BlueprintReadOnly)
	float LeanAmount;

	UPROPERTY()
	float YawChange;

	UPROPERTY()
	float PitchChange;
	// -----------------------

	/** Move variables */
	UPROPERTY()
	bool bIgnoreInputMoveVector = false;

	// -----------------------

	UPROPERTY()
	bool bTimeScale = false;

	/** Check if player can perform an attack in the air */
	bool bCanAirCombo = true;

	/** If player has been hit and thus cannot perform any action */
	bool bStaggered = false;

	UPROPERTY(EditAnywhere)
	UCurveTable* MaxHealthLevelCurve = nullptr;

	UPROPERTY(EditAnywhere)
	UCurveTable* AttackPowerLevelCurve = nullptr;
	// -----------------------------------------

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	URGX_LaunchEventDataAsset* MoveAwayLaunchPayload;

protected:
	/** Called for forwards/backwards input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	//** Animation Functions */
	FRGX_LeanInfo CalculateLeanAmount();

	void Landed(const FHitResult& Hit) override;

	UFUNCTION(BlueprintCallable)
	void OnCapsuleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// --- APawn interface ---
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PossessedBy(AController* NewController) override;
	// -----------------------

	// FGenericTeamId interface
	void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	FGenericTeamId GetGenericTeamId() const override;
	// End of FGenericTeamId interface

	// Combat input functions that redirect the managing of the input to the combat system passing 
	// the input pressed as the argument.
	void ManageLightAttackInput();
	void ManageLightAttackInputRelease();

	void ManageHeavyAttackInput();
	void ManageHeavyAttackInputRelease();

	void PerformFallAttack();
	void PerformLaunchAttack();
	void ChangePowerSkill();

	//void ManagePowerSkillInput();
	void TryToInteract();
	// ----------------------------------

	/* Level and experience*/
	//void LevelUp(const float NewLevel);
	// ----------------------

	// Debug
	void PrintDebugInformation();

	void ChangeTimeScale();
	// ----------------

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** GameplayTagAssetInterface methods */
	void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;
	bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;

	/** RX_GameplayTagInterface methods */
	void AddGameplayTag(const FGameplayTag& TagToAdd) override;
	void RemoveGameplayTag(const FGameplayTag& TagToRemove) override;

	/** Stops any combo logic. It should be called at any action that interrupts an ongoing combo from the Combo system. */
	UFUNCTION(BlueprintCallable)
	void OnInterrupted();

	/** Utility methods */
	UFUNCTION(BlueprintCallable)
	bool IsBeingAttacked();

	/* Input Handler calls this to let the player handle the action */
	UFUNCTION()
	void HandleAction(const ERGX_PlayerActions Action);
};