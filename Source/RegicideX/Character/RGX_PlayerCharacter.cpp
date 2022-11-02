#include "RGX_PlayerCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GenericTeamAgentInterface.h"
#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RegicideX/Components/RGX_CameraControllerComponent.h"
#include "RegicideX/Components/RGX_ComboSystemComponent.h"
#include "RegicideX/Components/RGX_HitboxComponent.h"
#include "RegicideX/Components/RGX_InputHandlerComponent.h"
#include "RegicideX/Components/RGX_InteractComponent.h"
#include "RegicideX/Components/RGX_MovementAssistComponent.h"
#include "RegicideX/GameplayFramework/RGX_PlayerState.h"
#include "RegicideX/GAS/AttributeSets/RGX_MovementAttributeSet.h"
#include "RegicideX/GAS/AttributeSets/RGX_ManaAttributeSet.h"
#include "RegicideX/Notifies/RGX_ANS_JumpComboSection.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "RegicideX/RGX_PlayerCameraManager.h"
#include "RegicideX/GAS/RGX_GameplayEffectContext.h"
#include "AbilitySystemGlobals.h"
#include "Components/WidgetComponent.h"
#include "RegicideX/GAS/GameplayAbilities/Common/RGX_GA_MeleeAttackAbility.h"
#include "Camera/CameraShakeBase.h"
#include "RegicideX/GAS/RGX_GameplayEffectContext.h"
#include "RegicideX/GAS/GameplayAbilities/BaseAbilities/RGX_GameplayAbility.h"

ARGX_PlayerCharacter::ARGX_PlayerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	// Set our turn rate for input
	BaseTurnRate = 45.0f;
	BaseLookUpRate = 45.0f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	InteractWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidgetComponent"));
	InteractWidgetComponent->SetupAttachment(FollowCamera);

	CameraControllerComponent	= CreateDefaultSubobject<URGX_CameraControllerComponent>(TEXT("CameraControllerComponent"));
	CameraControllerComponent->Camera = FollowCamera;
	CameraControllerComponent->SpringArm = CameraBoom;

	ComboSystemComponent		= CreateDefaultSubobject<URGX_ComboSystemComponent>(TEXT("ComboSystemComponent"));
	CombatAssistComponent		= CreateDefaultSubobject<URGX_CombatAssistComponent>(TEXT("CombatAssistComponent"));
	InputHandlerComponent		= CreateDefaultSubobject<URGX_InputHandlerComponent>(TEXT("InputHandlerComponent"));
	MovementAttributeSet		= CreateDefaultSubobject<URGX_MovementAttributeSet>(TEXT("MovementAttributeSet"));
	ManaAttributeSet			= CreateDefaultSubobject<URGX_ManaAttributeSet>(TEXT("ManaAttributeSet"));
	InteractComponent			= CreateDefaultSubobject<URGX_InteractComponent>(TEXT("InteractComponent"));
	MovementAssistComponent		= CreateDefaultSubobject<URGX_MovementAssistComponent>(TEXT("MovementAssistComponent"));

	InteractComponent->InteractWidgetComponent = InteractWidgetComponent;
}

void ARGX_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Debug", IE_Pressed, this, &ARGX_PlayerCharacter::PrintDebugInformation);

	PlayerInputComponent->BindAction("LightAttack", IE_Pressed, this, &ARGX_PlayerCharacter::ManageLightAttackInput);
	PlayerInputComponent->BindAction("LightAttack", IE_Released, this, &ARGX_PlayerCharacter::ManageLightAttackInputRelease);
	PlayerInputComponent->BindAction("HeavyAttack", IE_Pressed, this, &ARGX_PlayerCharacter::ManageHeavyAttackInput);
	PlayerInputComponent->BindAction("HeavyAttack", IE_Released, this, &ARGX_PlayerCharacter::ManageHeavyAttackInputRelease);
	PlayerInputComponent->BindAction("ShootSkill", IE_Pressed, this, &ARGX_PlayerCharacter::ManageSpearAttackInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARGX_PlayerCharacter::ManageJumpInput);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ARGX_PlayerCharacter::ManageJumpInputReleased);

	PlayerInputComponent->BindAction("TimeScale", IE_Pressed, this, &ARGX_PlayerCharacter::ChangeTimeScale);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ARGX_PlayerCharacter::TryToInteract);
	PlayerInputComponent->BindAction("ToggleTargeting", IE_Pressed, this, &ARGX_PlayerCharacter::ToggleTargeting);
	PlayerInputComponent->BindAction("EnableTargeting", IE_Pressed, this, &ARGX_PlayerCharacter::EnableTargeting);
	PlayerInputComponent->BindAction("EnableTargeting", IE_Released, this, &ARGX_PlayerCharacter::DisableTargeting);
	PlayerInputComponent->BindAction("TargetLeft", IE_Pressed, this, &ARGX_PlayerCharacter::TargetLeft);
	PlayerInputComponent->BindAction("TargetRight", IE_Pressed, this, &ARGX_PlayerCharacter::TargetRight);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARGX_PlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARGX_PlayerCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ARGX_PlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ARGX_PlayerCharacter::LookUpAtRate);

	AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"), FString("CancelTarget"), FString("ERGX_AbilityInputID"), static_cast<int32>(ERGX_AbilityInputID::Confirm), static_cast<int32>(ERGX_AbilityInputID::Cancel)));
}

void ARGX_PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AddGameplayTag(FGameplayTag::RequestGameplayTag("PossessedBy.Player"));
}

void ARGX_PlayerCharacter::RotateToTarget(float DeltaTime)
{
	if (bIsStrafing)
	{
		if (CameraControllerComponent->CurrentTarget.IsValid())
		{
			const ARGX_EnemyBase* target = CameraControllerComponent->CurrentTarget.Get();

			const FVector selfLocation = GetActorLocation();
			const FVector targetLocation = target->GetActorLocation();

			const FRotator selfRotation = GetActorRotation();
			const FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(selfLocation, targetLocation);
			const FRotator desiredRotation = FRotator(selfRotation.Pitch, lookRotation.Yaw, selfRotation.Roll);

			const FRotator finalRotation = FMath::Lerp(selfRotation, desiredRotation, RotationSpeedWhenStrafing*DeltaTime);
			SetActorRotation(finalRotation);
		}
	}
}

void ARGX_PlayerCharacter::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	CharacterTeam = TeamID;
}

FGenericTeamId ARGX_PlayerCharacter::GetGenericTeamId() const
{
	return CharacterTeam;
}

bool ARGX_PlayerCharacter::IsAttacking()
{
	TArray<FGameplayAbilitySpec*> ActivableAbilities;
	GetAbilitySystemComponent()->GetActivatableGameplayAbilitySpecsByAllMatchingTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Ability.Melee")), ActivableAbilities);
	for (const FGameplayAbilitySpec* Ability : ActivableAbilities)
	{
		if (Ability->IsActive())
			return true;
	}
	return false;
}

bool ARGX_PlayerCharacter::IsDashing()
{
	return HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.Dashing")));
}

void ARGX_PlayerCharacter::ManageLightAttackInput()
{
	if (bStaggered == true)
		return;

	InputHandlerComponent->HandleInput(ERGX_PlayerInputID::LightAttackInput, false, GetCharacterMovement()->IsFalling());

	// If we are performing an attack, try to follow the combo
	if (IsAttacking() || IsDashing())
	{
		if (JumpComboNotifyState != nullptr)
		{
			// If Input and CanCombo, signal player has pressed input.
			if (JumpComboNotifyState->InputID == ERGX_ComboTokenID::LightAttackToken && bCanCombo)
			{
				bContinueCombo = true;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("ComboTokenID was not LightAttackToken"));
			}
		}
	}
	else
	{
		if (GetCharacterMovement()->IsFalling() && bCanAirCombo == true)
		{
			UE_LOG(LogTemp, Warning, TEXT("CanAirCombo\n"));
			FGameplayEventData EventData;
			int32 TriggeredAbilities = AbilitySystemComponent->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("Combo.Air.Light")), &EventData);
			// clean state if ability was not activated
			if (TriggeredAbilities == 0)
			{
				bCanCombo = false;
				ComboSystemComponent->OnEndCombo();
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Remove Can Air Combo\n"));
				bCanAirCombo = false;
				StopJumping();
				LaunchCharacter(FVector(0.0f, 0.0f, -1.0f), true, true); // If Z force is 0.0f for some reason it doesn't work
				GetCharacterMovement()->GravityScale = 0.0f;
			}
		}
		else if (GetCharacterMovement()->IsFalling() == false)
		{
			FGameplayEventData EventData;
			int32 TriggeredAbilities = AbilitySystemComponent->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("Combo.Light")), &EventData);
			// clean state if ability was not activated
			if (TriggeredAbilities == 0)
			{
				bCanCombo = false;
				ComboSystemComponent->OnEndCombo();
			}
		}
	}
}

void ARGX_PlayerCharacter::ManageLightAttackInputRelease()
{
	if (bStaggered == true)
		return;

	InputHandlerComponent->HandleInput(ERGX_PlayerInputID::LightAttackInput, true, GetCharacterMovement()->IsFalling());
}

void ARGX_PlayerCharacter::ManageHeavyAttackInput()
{
	if (bStaggered == true)
		return;

	InputHandlerComponent->HandleInput(ERGX_PlayerInputID::HeavyAttackInput, false, GetCharacterMovement()->IsFalling());
}

void ARGX_PlayerCharacter::ManageHeavyAttackInputRelease()
{
	if (bStaggered == true)
		return;

	InputHandlerComponent->HandleInput(ERGX_PlayerInputID::HeavyAttackInput, true, GetCharacterMovement()->IsFalling());
}

void ARGX_PlayerCharacter::ManageJumpInput()
{
	// Jump cancels current attack
	FGameplayTagContainer TagContainer(FGameplayTag::RequestGameplayTag(FName("Ability.Melee")));
	GetAbilitySystemComponent()->CancelAbilities(&TagContainer);
	OnInterrupted();

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		// Reset collision with pawn after player has jumped
		FTimerDelegate TimerDel;
		FTimerHandle TimerHandle;
		TimerDel.BindUFunction(this, FName("ResetPawnCollisionResponse"));
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 0.25f, false);
	}

	Jump();
	OnJump();
}

void ARGX_PlayerCharacter::ManageJumpInputReleased()
{
	StopJumping();
}

void ARGX_PlayerCharacter::ManageSpearAttackInput()
{
	UE_LOG(LogTemp, Warning, TEXT("Managing Spear power!"));
	FGameplayEventData EventData;
	int32 TriggeredAbilities = AbilitySystemComponent->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("Power.Spears")), &EventData);
}

void ARGX_PlayerCharacter::TryToInteract()
{
	if (bStaggered == true)
		return;

	InteractComponent->TryToInteract();
}

bool ARGX_PlayerCharacter::IsBeingAttacked()
{
	const FVector PlayerLocation = GetActorLocation();

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(DodgeableObjectType);

	UClass* SeekClass = ARGX_EnemyBase::StaticClass();

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Init(this, 1);

	TArray<AActor*> OutActors;

	float OutRadius;
	float OutHalfHeight;

	GetCapsuleComponent()->GetScaledCapsuleSize(OutRadius, OutHalfHeight);

	// TODO: Use the capsule component of ACharacter to check if there is any hostile collider overlapping
	// Check hostile colliders that may be already overlapping us
	UKismetSystemLibrary::CapsuleOverlapActors(
		GetWorld(), PlayerLocation, OutRadius, OutHalfHeight, TraceObjectTypes, nullptr, IgnoreActors, OutActors);

	if (OutActors.Num() > 0)
	{
		return true;
	}

	UKismetSystemLibrary::CapsuleOverlapActors(
		GetWorld(), PlayerLocation, OutRadius * 4.0f, OutHalfHeight * 2.4f, TraceObjectTypes, nullptr, IgnoreActors, OutActors);

	for (AActor* HitActor : OutActors)
	{
		URGX_HitboxComponent* Hitbox = HitActor->FindComponentByClass<URGX_HitboxComponent>();
		if (Hitbox)
		{
			if (Hitbox->IsGoingToOverlapActor(this))
			{
				return true;
			}
		}
	}

	return false;
}

void ARGX_PlayerCharacter::OnFinishBrake()
{
	EndBrake();
}

void ARGX_PlayerCharacter::HandleAction(const ERGX_PlayerActions Action)
{
	switch(Action)
	{
	case ERGX_PlayerActions::LaunchAttack:
		PerformLaunchAttack();
		break;
	case ERGX_PlayerActions::FallAttack:
		PerformFallAttack();
		break;
	case ERGX_PlayerActions::HeavyAttack:
		PerformHeavyAttack();
		break;
	default:
		break;
	}
}

void ARGX_PlayerCharacter::UpdateMana(const float AddedMana)
{
	if (ManaAttributeSet->GetMana() + AddedMana >= ManaAttributeSet->GetMaxMana())
		{
		if (ManaAttributeSet->GetManaStack() == ManaAttributeSet->GetMaxManaStack())
		{
			ManaAttributeSet->SetMana(ManaAttributeSet->GetMaxMana());
			ManaAttributeSet->SetManaStack(ManaAttributeSet->GetMaxManaStack());
		}
		else
		{
			ManaAttributeSet->SetManaStack(ManaAttributeSet->GetManaStack() + 1);
			ManaAttributeSet->SetMana(0.0f);
			OnAddStack();
		}
	}
	else
	{
		ManaAttributeSet->SetMana(ManaAttributeSet->GetMana() + AddedMana);
	}
	OnUpdateMana();
}

void ARGX_PlayerCharacter::OnHitboxHit(UGameplayAbility* GameplayAbility, FGameplayEventData EventData, TSubclassOf<UCameraShakeBase> CameraShakeClass)
{
	URGX_MeleeAttackAbility* MeleeAbility = Cast<URGX_MeleeAttackAbility>(GameplayAbility);
	if (MeleeAbility)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
			if (CameraManager)
			{
				CameraManager->StartCameraShake(CameraShakeClass);
			}
		}
	}
}

void ARGX_PlayerCharacter::PerformFallAttack()
{
	FGameplayEventData EventData;
	int32 TriggeredAbilities = AbilitySystemComponent->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("Combo.Air.Takedown")), &EventData);
}

void ARGX_PlayerCharacter::PerformLaunchAttack()
{
	// Launch Attack
	FGameplayEventData EventData;
	int32 TriggeredAbilities = AbilitySystemComponent->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("Combo.Launch")), &EventData);
}

void ARGX_PlayerCharacter::PerformHeavyAttack()
{
	// If we are performing an attack, try to follow the combo
	if (IsAttacking())
	{
		if (JumpComboNotifyState != nullptr)
		{
			// Jump Section for combo
			if (JumpComboNotifyState->InputID == ERGX_ComboTokenID::HeavyAttackToken && bCanCombo)
			{
				bContinueCombo = true;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("ComboTokenID was not HeavyAttackToken"))
			}
		}
	}
	else
	{
		FGameplayEventData EventData;
		int32 TriggeredAbilities = AbilitySystemComponent->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("Combo.Heavy")), &EventData);
		// clean state if ability was not activated
		if (TriggeredAbilities == 0)
		{
			bCanCombo = false;
			ComboSystemComponent->OnEndCombo();
		}
	}
}

void ARGX_PlayerCharacter::ToggleTargeting()
{
	CameraControllerComponent->ToggleTargeting();
}

void ARGX_PlayerCharacter::EnableTargeting()
{
	CameraControllerComponent->EnableTargeting();
}

void ARGX_PlayerCharacter::DisableTargeting()
{
	CameraControllerComponent->DisableTargeting();
}

void ARGX_PlayerCharacter::TargetLeft()
{
	CameraControllerComponent->TargetLeft();
}

void ARGX_PlayerCharacter::TargetRight()
{
	CameraControllerComponent->TargetRight();
}

void ARGX_PlayerCharacter::CheckBrake(float DeltaTime)
{
	if (bIsBraking || bIsStrafing) return;

	FVector LastInputDirection = GetLastMoveInputDirection();
	FVector CurrentInputDirection = GetCurrentMoveInputDirection();

	if (LastInputDirection.IsNearlyZero() || CurrentInputDirection.IsNearlyZero()) return;

	UE_LOG(LogTemp, Warning, TEXT("LastInputDirection: %f,%f,%f"), LastInputDirection.X, LastInputDirection.Y, LastInputDirection.Z);
	UE_LOG(LogTemp, Warning, TEXT("CurrentInputDirection: %f,%f,%f"), CurrentInputDirection.X, CurrentInputDirection.Y, CurrentInputDirection.Z);

	const float Dot = FVector::DotProduct(LastInputDirection, CurrentInputDirection);
	bool bDotCondition = Dot < -0.5f;
	UE_LOG(LogTemp, Warning, TEXT("Dot: %f"), Dot);
	bool bVelocityThreshold = VelocityMagnitudeLastFrame > MinVelocityForBrake;
	if (bDotCondition && bVelocityThreshold == true && GetCharacterMovement()->IsFalling() == false)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Start Braking")));
		StartBrake();	
	}
}

void ARGX_PlayerCharacter::StartBrake()
{
	bIsBraking = true;
	return;

	// Brake cancels current attack
	FGameplayTagContainer TagContainer(FGameplayTag::RequestGameplayTag(FName("Ability.Melee")));
	GetAbilitySystemComponent()->CancelAbilities(&TagContainer);
	OnInterrupted();

	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	if (CharacterMovementComponent)
	{
		CharacterMovementComponent->MaxWalkSpeed = 0.0f;
		CharacterMovementComponent->RotationRate.Yaw = BrakeYawRotationRate;
	}
}

void ARGX_PlayerCharacter::EndBrake()
{
	return;

	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	if (CharacterMovementComponent)
	{
		CharacterMovementComponent->MaxWalkSpeed = GetCurrentMaxSpeed();
		CharacterMovementComponent->RotationRate.Yaw = 540.0f;
	}

	bIsBraking = false;
}

void ARGX_PlayerCharacter::PrintDebugInformation()
{
	UAbilitySystemComponent* ACS = GetAbilitySystemComponent();
	if (ACS)
	{
		TSubclassOf<UGameplayEffect> GameplayEffectToApply = AutoDamageEffectContextContainer.EffectToApply;
		FGameplayEffectContextHandle ContextHandle = ACS->MakeEffectContext();
		FRGX_GameplayEffectContext* RGXContext = static_cast<FRGX_GameplayEffectContext*>(ContextHandle.Get());
		RGXContext->OptionalObject = AutoDamageEffectContextContainer.Payload;
		ACS->ApplyGameplayEffectToTarget(GameplayEffectToApply->GetDefaultObject<UGameplayEffect>(), ACS, GetCharacterLevel(), ContextHandle);
	}
}

void ARGX_PlayerCharacter::ChangeTimeScale()
{
	if (bTimeScale == false)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.3f);
		bTimeScale = true;
	}
	else
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
		bTimeScale = false;
	}
}

void ARGX_PlayerCharacter::OnInterrupted()
{
	bCanJumpToComboSection = false;
	ComboSystemComponent->OnEndCombo();
	InputHandlerComponent->ResetAirState();
	InputHandlerComponent->ResetInputState();
}

void ARGX_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	AddGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.CanAirCombo")));

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ARGX_PlayerCharacter::OnCapsuleHit);

	CameraControllerComponent->OnTargetUpdated.__Internal_AddUniqueDynamic(this, &ARGX_PlayerCharacter::OnTargetUpdatedImpl, "OnTargetUpdatedImpl");
	CameraControllerComponent->OnTargetUpdated.__Internal_AddUniqueDynamic(CombatAssistComponent, &URGX_CombatAssistComponent::SetTargetFromOutside, "SetTargetFromOutside");
}

void ARGX_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bool bWasStaggered = bStaggered;
	bStaggered = HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Combat.InHurtReact")));

	if (bWasStaggered == false && bStaggered == true)
	{
		InputHandlerComponent->ResetInputState();
	}

	//CheckBrake(DeltaTime);
	RotateToTarget(DeltaTime);
	UpdateStrafingSpeed();
	
	VelocityMagnitudeLastFrame = GetVelocity().Size();
	LastMoveInput = CurrentMoveInput;
}

UAbilitySystemComponent* ARGX_PlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ARGX_PlayerCharacter::DisableMovementInput()
{
	bIgnoreInputMoveVector = true;
}

void ARGX_PlayerCharacter::EnableMovementInput()
{
	bIgnoreInputMoveVector = false;
}

void ARGX_PlayerCharacter::OnFollowCombo()
{
	ComboSystemComponent->OnCombo();

	FGameplayTag NextAttack = ComboSystemComponent->GetNextAttack();
	if (NextAttack != FGameplayTag::RequestGameplayTag("Combo.None"))
	{
		FString NextAttackString = NextAttack.ToString();
		// Fire next attack
		FGameplayEventData EventData;
		int32 TriggeredAbilities = AbilitySystemComponent->HandleGameplayEvent(NextAttack, &EventData);
		// Clear next attack status
		ComboSystemComponent->CleanStatus(TriggeredAbilities);
	}
}

FVector ARGX_PlayerCharacter::GetCurrentMoveInputDirection()
{
	return GetMoveDirectionFromVector(CurrentMoveInput);
}

FVector ARGX_PlayerCharacter::GetLastMoveInputDirection()
{
	return GetMoveDirectionFromVector(LastMoveInput);
}

FVector ARGX_PlayerCharacter::GetMoveDirectionFromVector(FVector2D& InputVector)
{
	if (InputVector != FVector2D::ZeroVector)
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		FVector Direction = ForwardDirection * InputVector.X + RightDirection * InputVector.Y;
		Direction.Normalize();

		return Direction;
	}

	return FVector::ZeroVector;
}

void ARGX_PlayerCharacter::RotatePlayerTowardsInput()
{
	FVector InputDirection = GetCurrentMoveInputDirection();

	if (InputDirection.IsNearlyZero() == false)
	{
		SetActorRotation(InputDirection.Rotation());
	}
}

void ARGX_PlayerCharacter::HandleDamage(
	float DamageAmount, 
	const FHitResult& HitInfo, 
	const FGameplayTagContainer& DamageTags, 
	ARGX_CharacterBase* InstigatorCharacter, 
	AActor* DamageCauser, 
	ERGX_AnimEvent HitReactFlag)
{
	Super::HandleDamage(DamageAmount, HitInfo, DamageTags, InstigatorCharacter, DamageCauser, HitReactFlag);

	StopAnimMontage();

	if (IsAlive())
	{
		UAnimMontage* AnimToPlay = nullptr;
		ERGX_AnimEvent AnimEventFlag = HitReactFlag;

		UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
		if (CharacterMovementComponent)
		{
			if(CharacterMovementComponent->IsFalling())
			{
				AnimEventFlag = ERGX_AnimEvent::KnockbackHitReact;
			}
		}

		const FAnimationArray AnimationList = *AnimMontageMap.Find(AnimEventFlag);
		
		if (AnimationList.Animations.Num() > 0)
		{
			int32 Index = UKismetMathLibrary::RandomIntegerInRange(0, AnimationList.Animations.Num() - 1);
			AnimToPlay = AnimationList.Animations[Index];
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Not animations assigned"));
			return;
		}

		PlayAnimMontage(AnimToPlay);
	}
	else
	{
		DisableInput(Cast<APlayerController>(GetController()));

		UAnimMontage* AnimToPlay = nullptr;
		const FAnimationArray AnimationList = *AnimMontageMap.Find(ERGX_AnimEvent::Death);
		if (AnimationList.Animations.Num() > 0)
		{
			int32 Index = UKismetMathLibrary::RandomIntegerInRange(0, AnimationList.Animations.Num() - 1);
			AnimToPlay = AnimationList.Animations[Index];
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Not animations assigned"));
			return;
		}
		PlayAnimMontage(AnimToPlay);
	}
}

void ARGX_PlayerCharacter::MoveForward(float Value)
{
	CurrentMoveInput.X = 0.0f;

	const bool bIsControllerNull = Controller == nullptr;
	const bool bIsAxisValueLesserThanThreshold = std::abs(Value) <= 0.0;
	if (bStaggered || bIsControllerNull || bIsAxisValueLesserThanThreshold || bIgnoreInputMoveVector)
		return;

	FVector direction;
	// find out which way is forward
	if (bIsStrafing)
	{
		direction = GetActorForwardVector();
	}
	else
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		// get forward vector
		//const FVector Direction = UKismetMathLibrary::GetForwardVector(YawRotation);
		direction = UKismetMathLibrary::GetForwardVector(YawRotation);
	}

	// add movement in that direction
	AddMovementInput(direction, Value);
	CurrentMoveInput.X = Value;
}

void ARGX_PlayerCharacter::MoveRight(float Value)
{
	CurrentMoveInput.Y = 0.0f;

	const bool bIsControllerNull = Controller == nullptr;
	const bool bIsAxisValueLesserThanThreshold = std::abs(Value) <= 0.0;
	if (bStaggered || bIsControllerNull || bIsAxisValueLesserThanThreshold || bIgnoreInputMoveVector)
		return;

	FVector direction;
	// find out which way is forward
	if (bIsStrafing)
	{
		direction = GetActorRightVector();
	}
	else
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		// get right vector
		//const FVector Direction = UKismetMathLibrary::GetRightVector(YawRotation);
		direction = UKismetMathLibrary::GetRightVector(YawRotation);
	}

	// add movement in that direction
	AddMovementInput(direction, Value);
	CurrentMoveInput.Y = Value;
}

void ARGX_PlayerCharacter::TurnAtRate(float Rate)
{
	// TODO: Only TurnAtRate or AddControllerYawInput should modify YawChange at a time, depending if the user is using mouse or controller
	CameraControllerComponent->CheckYawInput(Rate);
	float YawFinalChange = Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds();
	Super::AddControllerYawInput(YawFinalChange);
}

void ARGX_PlayerCharacter::LookUpAtRate(float Rate)
{
	PitchChange = Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds();
	Super::AddControllerPitchInput(PitchChange);
}

void ARGX_PlayerCharacter::AddControllerYawInput(float Val)
{
	Super::AddControllerYawInput(Val);
	YawChange = Val;
}

void ARGX_PlayerCharacter::AddControllerPitchInput(float Val)
{
	Super::AddControllerPitchInput(Val);
}

void ARGX_PlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	UPrimitiveComponent* PrimitiveComponent = Hit.GetComponent();
	if (PrimitiveComponent)
	{
		ECollisionChannel CollisionChannel = PrimitiveComponent->GetCollisionObjectType();
		if (CollisionChannel == ECollisionChannel::ECC_WorldStatic)
		{
			if (GetCapsuleComponent())
			{
				GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
			}

			InputHandlerComponent->ResetAirState();

			AddGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.CanAirCombo")));
			RemoveGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.HasAirDashed")));
			bCanAirCombo = true;
		}
	}
}

void ARGX_PlayerCharacter::OnHandleEndKnockedUp()
{
	if (IsAttacking() == false && IsDashing() == false)
	{
		ResetGravity();
	}
}

void ARGX_PlayerCharacter::OnCapsuleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bWasFallingDownThisFrame)
	{
		ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(OtherActor);
		if (Enemy)
		{
			const FVector Normal = Hit.Normal;
			const FVector PlayerLaunchForce = Normal * FVector(1.0f, 1.0f, -1.0f) * 100.0f;

			GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

			LaunchCharacter(PlayerLaunchForce, true, true);

			UAbilitySystemComponent* OtherACS = Enemy->FindComponentByClass<UAbilitySystemComponent>();
			if (OtherACS)
			{
				FGameplayEventData EventData;
				EventData.Instigator = this;
				EventData.EventTag = MoveAwayLaunchPayload->GetEventTag();
				EventData.OptionalObject = MoveAwayLaunchPayload;
				OtherACS->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Launched")), &EventData);
			}
		}
	}
}

void ARGX_PlayerCharacter::OnTargetUpdatedImpl(ARGX_EnemyBase* NewTarget)
{
	bIsStrafing = NewTarget != nullptr;

	if (bIsStrafing)
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		//GetCharacterMovement()->MaxWalkSpeed = StrafingSpeed;
		//GetCharacterMovement()->MaxAcceleration = StrafingAcceleration;
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
		GetCharacterMovement()->MaxAcceleration = MaxAcceleration;
	}
}

void ARGX_PlayerCharacter::OnJump_Implementation()
{

}

void ARGX_PlayerCharacter::UpdateStrafingSpeed()
{
	if (bIsStrafing)
	{
		float direction = 0.0f;

		const FVector velocity = GetVelocity();
		if (!velocity.IsNearlyZero())
		{
			FMatrix RotMatrix = FRotationMatrix(GetActorRotation());
			FVector ForwardVector = RotMatrix.GetScaledAxis(EAxis::X);
			FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y);
			FVector NormalizedVel = velocity.GetSafeNormal2D();

			// get a cos(alpha) of forward vector vs velocity
			float ForwardCosAngle = FVector::DotProduct(ForwardVector, NormalizedVel);
			// now get the alpha and convert to degree
			float ForwardDeltaDegree = FMath::RadiansToDegrees(FMath::Acos(ForwardCosAngle));

			// depending on where right vector is, flip it
			float RightCosAngle = FVector::DotProduct(RightVector, NormalizedVel);
			if (RightCosAngle < 0)
			{
				ForwardDeltaDegree *= -1;
			}

			direction = ForwardDeltaDegree;
		}

		if (fabs(direction) < AngleToActivateStrafingSpeed)
		{
			GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = StrafingSpeed;
		}
	}
}

void ARGX_PlayerCharacter::ResetPawnCollisionResponse()
{	
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	}
}
