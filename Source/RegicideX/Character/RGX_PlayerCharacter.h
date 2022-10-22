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
class URGX_ManaAttributeSet;
class URGX_InteractComponent;
class URGX_LaunchEventDataAsset;
class UGameplayEffect;
class UWidgetComponent;

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

	/** Camera Targetting Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class URGX_CameraControllerComponent* CameraControllerComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class URGX_MovementAssistComponent* MovementAssistComponent = nullptr;

	// Attributes ---------------
	UPROPERTY()
	URGX_MovementAttributeSet* MovementAttributeSet = nullptr;

	UPROPERTY()
	URGX_ManaAttributeSet* ManaAttributeSet = nullptr;

	// --------------------------
public:
	ARGX_PlayerCharacter();

	/** Check if player is attacking, meaning the player has an active ability with Ability.Melee tag on it. */
	bool IsAttacking();

	bool IsDashing();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Strafing)
	float StrafingSpeed = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Strafing)
	float StrafingAcceleration = 2000.f;

	UPROPERTY()
	uint8 CurrentSkillSelected = 0;

	UPROPERTY()
	FGameplayTag CurrentSkillTag;

	/** If true, player is in window to carry on with the combo if appropiate input is pressed. */
	UPROPERTY(BlueprintReadWrite)
	bool bCanCombo = false;

	/** Signals if player has pressed an input to continue the on going combo. */
	UPROPERTY(BlueprintReadWrite)
	bool bContinueCombo = false;

	/** If true, we can jump to next section in the combo. */
	UPROPERTY(BlueprintReadWrite)
	bool bCanJumpToComboSection = false;

	/** Holds the AnimNotifyState of the current attack, which has the information for the combo to follow. */
	UPROPERTY()
	URGX_ANS_JumpComboSection* JumpComboNotifyState;

	//--------------------------

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EObjectTypeQuery> DodgeableObjectType;

	UPROPERTY()
	bool bIsBraking;

	UPROPERTY()
	bool bIsStrafing = false;

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

	UFUNCTION(BlueprintCallable)
	FVector GetCurrentMoveInputDirection();

	UFUNCTION(BlueprintCallable)
	FVector GetLastMoveInputDirection();

	UFUNCTION(BlueprintCallable)
	FVector GetMoveDirectionFromVector(FVector2D& InputVector);

	UFUNCTION(BlueprintCallable)
	void RotatePlayerTowardsInput();

	float GetCurrentMaxSpeed() const override;

	/** Events called from attribute set changes to decouple the logic. They call BP events. */
	virtual void HandleDamage(
		float DamageAmount,
		const FHitResult& HitInfo,
		const struct FGameplayTagContainer& DamageTags,
		ARGX_CharacterBase* InstigatorCharacter,
		AActor* DamageCauser,
		ERGX_AnimEvent HitReactFlag) override;

	FVector2D LastMoveInput = FVector2D::ZeroVector;
	FVector2D CurrentMoveInput = FVector2D::ZeroVector;

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

	UPROPERTY(EditDefaultsOnly)
	float MinVelocityForBrake = 400.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float BrakeYawRotationRate = 240.0f;

	float VelocityMagnitudeLastFrame;
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

	virtual void AddControllerYawInput(float Val) override;

	virtual void AddControllerPitchInput(float Val) override;

	void Landed(const FHitResult& Hit) override;

	virtual void OnHandleEndKnockedUp() override;

	UFUNCTION(BlueprintCallable)
	void OnCapsuleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnTargetUpdatedImpl(ARGX_EnemyBase* NewTarget);

protected:
	// --- APawn interface ---
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PossessedBy(AController* NewController) override;
	// -----------------------

	void RotateToTarget(float DeltaTime);

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

	void ManageJumpInput();
	void ManageJumpInputReleased();

	void PerformFallAttack();
	void PerformLaunchAttack();
	void PerformHeavyAttack();

	void ToggleTargeting();
	void EnableTargeting();
	void DisableTargeting();
	void TargetLeft();
	void TargetRight();

	void CheckBrake(float DeltaTime);
	void StartBrake();
	void EndBrake();

	void TryToInteract();

	// Debug
	void PrintDebugInformation();

	void ChangeTimeScale();
	// ----------------

public:
	/** Stops any combo logic. It should be called at any action that interrupts an ongoing combo from the Combo system. */
	UFUNCTION(BlueprintCallable)
	void OnInterrupted();

	/** Utility methods */
	UFUNCTION(BlueprintCallable)
	bool IsBeingAttacked();

	UFUNCTION(BlueprintCallable)
	void OnFinishBrake();

	/* Input Handler calls this to let the player handle the action */
	UFUNCTION()
	void HandleAction(const ERGX_PlayerActions Action);

	/* Called when hitting an enemy. By default mana is increased by 10.0f but we should change such input mana calculations when needed.*/
	UFUNCTION()
	void UpdateMana(const float AddedMana = 10.0f);

	UFUNCTION(BlueprintImplementableEvent)
	void OnAddStack();

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateMana();

	virtual void OnHitboxHit(UGameplayAbility* MeleeAbility, FGameplayEventData EventData, TSubclassOf<UCameraShakeBase> CameraShakeClass) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE float GetYawChange() const { return YawChange; }
};