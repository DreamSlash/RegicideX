#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTags.h"
#include "AbilitySystemInterface.h"
#include "Components/MCV_AbilitySystemComponent.h"
#include "RegicideX/Components/RGX_CombatAssistComponent.h"
#include "RegicideX/Interfaces/RGX_GameplayTagInterface.h"
#include "RegicideX/Enums/RGX_InputEnums.h"
#include "GenericTeamAgentInterface.h"

#include "RGX_PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class URGX_AbilitySystemComponent;
class URGX_ComboSystemComponent;
class URGX_CombatAssistComponent;
class URGX_InputHandlerComponent;
class URGX_HealthAttributeSet;
class URGX_MovementAttributeSet;
class URGX_CombatAttributeSet;
class URGX_InteractComponent;
class UGameplayEffect;

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
class REGICIDEX_API ARGX_PlayerCharacter : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface, public IRGX_GameplayTagInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera = nullptr;

	/** Ability System Component to be used */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	UMCV_AbilitySystemComponent* AbilitySystemComponent = nullptr;

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

	// Attributes ---------------
	UPROPERTY()
	URGX_HealthAttributeSet* HealthAttributeSet = nullptr;

	UPROPERTY()
	URGX_CombatAttributeSet* CombatAttributeSet = nullptr;

	UPROPERTY()
	URGX_MovementAttributeSet* MovementAttributeSet = nullptr;

	// --------------------------
public:
	ARGX_PlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGenericTeamId CharacterTeam;

	// TODO [REFACTOR]: Move this to AbilitySystemComponent.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FGameplayTag> PowerSkills;

	UPROPERTY()
	uint8 CurrentSkillSelected = 0;

	UPROPERTY()
	FGameplayTag CurrentSkillTag;
	//--------------------------

	/* Level used to determine attributes and gameplay abilities stats */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0.0", UIMin = "0.0"))
	int Level = 1;

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

	/** Input Variables */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float FallAttackHoldTime = 0.15f;
	//bool bFallAttackFlag = false;

	float HeavyInputCurrentHoldTime = 0.0f;

	UPROPERTY()
	bool bHeavyInputFlag = false;
	bool bHeavyInputPressedInAir = false;
	// -------------------

	/* Level Up variables */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> LevelUpEffect;

	UPROPERTY(EditAnywhere)
	UCurveTable* MaxHealthLevelCurve = nullptr;

	UPROPERTY(EditAnywhere)
	UCurveTable* AttackPowerLevelCurve = nullptr;
	// -----------------------------------------

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

	void ManagePowerSkillInput();
	void TryToInteract();
	// ----------------------------------

	void PerformFallAttack();
	void PerformLaunchAttack();
	void ChangePowerSkill();

	/* Level and experience*/
	void LevelUp(const float NewLevel);
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

	/** Utility methods */
	UFUNCTION(BlueprintCallable)
	bool IsBeingAttacked();

	/* Input Handler calls this to let the player handle the action */
	UFUNCTION()
	void HandleAction(const ERGX_PlayerActions Action);
};