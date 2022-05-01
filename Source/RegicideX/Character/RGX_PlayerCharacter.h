#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTags.h"
#include "AbilitySystemInterface.h"
#include "Components/MCV_AbilitySystemComponent.h"
#include "../Interfaces/RGX_GameplayTagInterface.h"
#include "../Enums/RGX_InputEnums.h"
#include "GenericTeamAgentInterface.h"

#include "RGX_PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class URGX_AbilitySystemComponent;
class URGX_ComboSystemComponent;
class URGX_HealthAttributeSet;
class URGX_CombatAttributeSet;

UCLASS(config = Game)
class REGICIDEX_API ARGX_PlayerCharacter : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface, public IRGX_GameplayTagInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Ability System Component to be used */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UMCV_AbilitySystemComponent* AbilitySystemComponent;

	/** Combo System Component to manage player combos */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combos, meta = (AllowPrivateAccess = "true"))
	URGX_ComboSystemComponent* ComboSystemComponent;

	// Attributes ---------------
	UPROPERTY()
	URGX_HealthAttributeSet* HealthAttributeSet;

	UPROPERTY()
	URGX_CombatAttributeSet* CombatAttributeSet;
	// --------------------------
public:
	ARGX_PlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGenericTeamId CharacterTeam;

	// TODO: Make a component to manage skills?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FGameplayTag> PowerSkills;

	UPROPERTY()
	uint8 CurrentSkillSelected = 0;

	UPROPERTY()
	FGameplayTag CurrentSkillTag;
	//--------------------------

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

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

protected:
	// --- APawn interface ---
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;
	// -----------------------

	// FGenericTeamId interface
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	// End of FGenericTeamId interface

	// Combat input functions that redirect the managing of the input to the combat system passing 
	// the input pressed as the argument.
	void ManageLightAttackInput();

	void ManageHeavyAttackInput();

	void ManagePowerSkillInput();
	// ----------------------------------

	UFUNCTION(BlueprintCallable)
	void PerformAttackAutoAssist();

	void ChangePowerSkill();

	// Debug
	void PrintDebugInformation();
	// ----------------

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** GameplayTagAssetInterface methods */
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;
	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;

	/** RX_GameplayTagInterface methods */
	virtual void AddGameplayTag(const FGameplayTag& TagToAdd) override;
	virtual void RemoveGameplayTag(const FGameplayTag& TagToRemove) override;
};