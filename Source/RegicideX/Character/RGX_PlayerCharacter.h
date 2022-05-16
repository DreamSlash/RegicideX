#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTags.h"
#include "AbilitySystemInterface.h"
#include "Components/MCV_AbilitySystemComponent.h"
#include "../Components/RGX_CombatAssistComponent.h"
#include "../Interfaces/RGX_GameplayTagInterface.h"
#include "../Enums/RGX_InputEnums.h"
#include "GenericTeamAgentInterface.h"

#include "RGX_PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class URGX_AbilitySystemComponent;
class URGX_ComboSystemComponent;
class URGX_CombatAssistComponent;
class URGX_HealthAttributeSet;
class URGX_MovementAttributeSet;
class URGX_CombatAttributeSet;

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

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EObjectTypeQuery> DodgeableObjectType;

	UPROPERTY(EditDefaultsOnly)
	float DefaultGravity = 3.0f;

	UPROPERTY(EditDefaultsOnly)
	float MaxWalkSpeed = 600.0f;

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
	// -------------------

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

	void ManageHeavyAttackInput();

	void ManagePowerSkillInput();
	// ----------------------------------

	void ChangePowerSkill();

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
};