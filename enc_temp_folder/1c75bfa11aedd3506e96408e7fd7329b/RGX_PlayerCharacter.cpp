#include "RGX_PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GenericTeamAgentInterface.h"
#include "Components/InputComponent.h"
#include "RegicideX/GameplayFramework/RGX_PlayerState.h" // TODO: write path to project settings
#include "RegicideX/Components/RGX_ComboSystemComponent.h"
#include "RegicideX/Components/RGX_HitboxComponent.h"
#include "RegicideX/Components/RGX_InputHandlerComponent.h"
#include "RegicideX/Components/RGX_InteractComponent.h"
#include "RegicideX/GAS/AttributeSets/RGX_HealthAttributeSet.h"
#include "RegicideX/GAS/AttributeSets/RGX_MovementAttributeSet.h"
#include "RegicideX/GAS/AttributeSets/RGX_CombatAttributeSet.h"
#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "RegicideX/RGX_PlayerCameraManager.h"
#include "RegicideX/GAS/RGX_GameplayEffectContext.h"

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
	GetCharacterMovement()->GravityScale = DefaultGravity;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;

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

	AbilitySystemComponent = CreateDefaultSubobject<UMCV_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	ComboSystemComponent = CreateDefaultSubobject<URGX_ComboSystemComponent>(TEXT("ComboSystemComponent"));
	CombatAssistComponent = CreateDefaultSubobject<URGX_CombatAssistComponent>(TEXT("CombatAssistComponent"));
	InputHandlerComponent = CreateDefaultSubobject<URGX_InputHandlerComponent>(TEXT("InputHandlerComponent"));
	InteractComponent = CreateDefaultSubobject<URGX_InteractComponent>(TEXT("InteractComponent"));
	HealthAttributeSet = CreateDefaultSubobject<URGX_HealthAttributeSet>(TEXT("HealthAttributeSet"));
	MovementAttributeSet = CreateDefaultSubobject<URGX_MovementAttributeSet>(TEXT("MovementAttributeSet"));
	CombatAttributeSet = CreateDefaultSubobject<URGX_CombatAttributeSet>(TEXT("CombatAttributeSet"));
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

	PlayerInputComponent->BindAction("SwitchPowerSkill", IE_Pressed, this, &ARGX_PlayerCharacter::ChangePowerSkill);
	PlayerInputComponent->BindAction("TimeScale", IE_Pressed, this, &ARGX_PlayerCharacter::ChangeTimeScale);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ARGX_PlayerCharacter::TryToInteract);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARGX_PlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARGX_PlayerCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ARGX_PlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ARGX_PlayerCharacter::LookUpAtRate);


	AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"), FString("CancelTarget"), FString("EMCVAbilityInputID"), static_cast<int32>(EMCVAbilityInputID::Confirm), static_cast<int32>(EMCVAbilityInputID::Cancel)));
}

void ARGX_PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AddGameplayTag(FGameplayTag::RequestGameplayTag("PossessedBy.Player"));
}

void ARGX_PlayerCharacter::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	CharacterTeam = TeamID;
}

FGenericTeamId ARGX_PlayerCharacter::GetGenericTeamId() const
{
	return CharacterTeam;
}

void ARGX_PlayerCharacter::ManageLightAttackInput()
{
	InputHandlerComponent->HandleInput(ERGX_PlayerInputID::LightAttackInput, false, GetCharacterMovement()->IsFalling());

	bool bCanAirCombo = HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.CanAirCombo")));
	FGameplayTag NextAttack = ComboSystemComponent->ManageInputToken(ERGX_ComboTokenID::LightAttackToken, GetCharacterMovement()->IsFalling(), bCanAirCombo);

	if (NextAttack == FGameplayTag::RequestGameplayTag(FName("Combo.Light")) || NextAttack == FGameplayTag::RequestGameplayTag(FName("Combo.Air.Light")))
	{
		FGameplayEventData EventData;
		int32 TriggeredAbilities = AbilitySystemComponent->HandleGameplayEvent(NextAttack, &EventData);
		UE_LOG(LogTemp, Warning, TEXT("Triggered Abilities: %d\n"), TriggeredAbilities);
		// clean state if ability was not activated
		if (TriggeredAbilities == 0)
		{
			ComboSystemComponent->OnEndCombo();
		}
		else if (GetCharacterMovement()->IsFalling() && bCanAirCombo == true)
		{
			UE_LOG(LogTemp, Warning, TEXT("Remove Can Air Combo\n"));
			RemoveGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.CanAirCombo")));
			StopJumping();
			LaunchCharacter(FVector(0.0f, 0.0f, -1.0f), true, true); // If Z force is 0.0f for some reason it doesn't work
			GetCharacterMovement()->GravityScale = 0.0f;
		}
	}
}

void ARGX_PlayerCharacter::ManageLightAttackInputRelease()
{
	InputHandlerComponent->HandleInput(ERGX_PlayerInputID::LightAttackInput, true, GetCharacterMovement()->IsFalling());
}

void ARGX_PlayerCharacter::ManageHeavyAttackInput()
{
	InputHandlerComponent->HandleInput(ERGX_PlayerInputID::HeavyAttackInput, false, GetCharacterMovement()->IsFalling());

	bHeavyInputFlag = true;
	bHeavyInputPressedInAir = GetCharacterMovement()->IsFalling() ? true : false;

	/*
	bool bCanAirCombo = HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.CanAirCombo")));
	FGameplayTag NextAttack = ComboSystemComponent->ManageInputToken(ERGXPlayerInputID::HeavyAttackInput, GetCharacterMovement()->IsFalling(), bCanAirCombo);

	if (NextAttack == FGameplayTag::RequestGameplayTag(FName("Combo.Heavy")))
	{
		FGameplayEventData EventData;
		int32 TriggeredAbilities = AbilitySystemComponent->HandleGameplayEvent(NextAttack, &EventData);
		UE_LOG(LogTemp, Warning, TEXT("Triggered Abilities: %d\n"), TriggeredAbilities);
		// clean state if ability was not activated
		if (TriggeredAbilities == 0)
		{
			ComboSystemComponent->OnEndCombo();
		}
	}
	*/
}

void ARGX_PlayerCharacter::ManageHeavyAttackInputRelease()
{
	InputHandlerComponent->HandleInput(ERGX_PlayerInputID::HeavyAttackInput, true, GetCharacterMovement()->IsFalling());

	/*
	if (GetCharacterMovement()->IsFalling() == false && bHeavyInputFlag == true)
	{
		// Launch Attack
		FGameplayEventData EventData;
		int32 TriggeredAbilities = AbilitySystemComponent->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("Combo.Launch")), &EventData);

		UE_LOG(LogTemp, Warning, TEXT("Triggered Abilities: %d\n"), TriggeredAbilities);
	}
	*/

	HeavyInputCurrentHoldTime = 0.0f;
	bHeavyInputFlag = false;
}

void ARGX_PlayerCharacter::ManagePowerSkillInput()
{
	// TODO: Make a component for managing skills?

	if (PowerSkills.Num() <= CurrentSkillSelected)
	{
		return;
	}

	// Fire next attack
	FGameplayEventData EventData;
	AbilitySystemComponent->HandleGameplayEvent(PowerSkills[CurrentSkillSelected], &EventData);
}

void ARGX_PlayerCharacter::TryToInteract()
{
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

	// Check dynamic colliders that potentially will hit us in the next frames
	UKismetSystemLibrary::CapsuleOverlapActors(
		GetWorld(), PlayerLocation, OutRadius * 1.5f, OutHalfHeight * 1.2f, TraceObjectTypes, nullptr, IgnoreActors, OutActors);

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

void ARGX_PlayerCharacter::HandleAction(const ERGX_PlayerActions Action)
{
	switch(Action)
	{
	case ERGX_PlayerActions::LaunchAttack:
		UE_LOG(LogTemp, Warning, TEXT("LaunchAttack\n"));
		PerformLaunchAttack();
		break;
	case ERGX_PlayerActions::FallAttack:
		UE_LOG(LogTemp, Warning, TEXT("FallAttack\n"));
		PerformFallAttack();
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Manuela\n"));
		break;
	}
}

void ARGX_PlayerCharacter::PerformFallAttack()
{
	FGameplayEventData EventData;
	int32 TriggeredAbilities = AbilitySystemComponent->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("Combo.Air.Takedown")), &EventData);

	bHeavyInputFlag = false;
	bHeavyInputPressedInAir = false;

	UE_LOG(LogTemp, Warning, TEXT("Triggered Abilities: %d\n"), TriggeredAbilities);
}

void ARGX_PlayerCharacter::PerformLaunchAttack()
{
	// Launch Attack
	FGameplayEventData EventData;
	int32 TriggeredAbilities = AbilitySystemComponent->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("Combo.Launch")), &EventData);

	bHeavyInputFlag = false;
	bHeavyInputPressedInAir = false;

	UE_LOG(LogTemp, Warning, TEXT("Triggered Abilities: %d\n"), TriggeredAbilities);
}

void ARGX_PlayerCharacter::ChangePowerSkill()
{
	if (PowerSkills.Num() < 2)
		return;

	RemoveGameplayTag(PowerSkills[CurrentSkillSelected]);
	CurrentSkillSelected++;

	if (CurrentSkillSelected == PowerSkills.Num())
	{
		CurrentSkillSelected = 0;
	}

	AddGameplayTag(PowerSkills[CurrentSkillSelected]);

	FString SkillName = PowerSkills[CurrentSkillSelected].ToString();
	UE_LOG(LogTemp, Warning, TEXT("Power Skill Selected: %s\n"), *SkillName);
}

void ARGX_PlayerCharacter::LevelUp(const float NewLevel)
{
	/*
	Level = NewLevel;

	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	FRGX_GameplayEffectContext* FRGXContext = static_cast<FRGX_GameplayEffectContext*>(ContextHandle.Get());

	FString ContextString;
	FRealCurve* MaxHealth = MaxHealthLevelCurve->FindCurve(FName("MaxHealth"), ContextString);
	FRealCurve* AttackPower = AttackPowerLevelCurve->FindCurve(FName("AttackPower"), ContextString);
	FRGXContext->NewMaxHealth = MaxHealth->Eval(Level);
	FRGXContext->NewAttackPower = AttackPower->Eval(Level);
	AbilitySystemComponent->ApplyGameplayEffectToSelf(LevelUpEffect.GetDefaultObject(), 1.0f, ContextHandle);
	*/
}

void ARGX_PlayerCharacter::PrintDebugInformation()
{
	TArray<FGameplayAttribute> attributes;
	AbilitySystemComponent->GetAllAttributes(attributes);

	/*
	for (FGameplayAttribute& attribute : attributes)
	{
		FString AttributeName = attribute.GetName();
		UE_LOG(LogTemp, Warning, TEXT("Attribute Name: %s\n"), *AttributeName);

		float AttributeValue = AbilitySystemComponent->GetNumericAttribute(attribute);
		UE_LOG(LogTemp, Warning, TEXT("Attribute Value: %f\n"), AttributeValue);
	}

	bool bHasTag = HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Power.Spears"));
	if (bHasTag)
	{
		UE_LOG(LogTemp, Warning, TEXT("Has Power Spears tag: TRUE\n"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Has Power Spears tag: FALSE\n"));
	}
	*/

	ComboSystemComponent->DrawDebugInfo();
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

void ARGX_PlayerCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	AbilitySystemComponent->GetOwnedGameplayTags(TagContainer);
}

bool ARGX_PlayerCharacter::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	return AbilitySystemComponent->HasMatchingGameplayTag(TagToCheck);
}

bool ARGX_PlayerCharacter::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystemComponent->HasAllMatchingGameplayTags(TagContainer);
}

bool ARGX_PlayerCharacter::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystemComponent->HasAnyMatchingGameplayTags(TagContainer);
}

void ARGX_PlayerCharacter::AddGameplayTag(const FGameplayTag& TagToAdd)
{
	AbilitySystemComponent->AddLooseGameplayTag(TagToAdd);
}

void ARGX_PlayerCharacter::RemoveGameplayTag(const FGameplayTag& TagToRemove)
{
	AbilitySystemComponent->RemoveLooseGameplayTag(TagToRemove);
}

void ARGX_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	AddGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.CanAirCombo")));

	if (PowerSkills.Num() < 1 == false)
	{
		CurrentSkillTag = PowerSkills[0];
		AddGameplayTag(CurrentSkillTag);
	}

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ARGX_PlayerCharacter::OnCapsuleHit);

	//LevelUp(Level);
}

void ARGX_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (GetVelocity().Z < 0)
	{
		bIsFallingDown = true;
	}
	/*
	if (bIsFallingDown == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("Is Falling Down: TRUE\n"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Is Falling Down: FALSE\n"));
	}
	*/
	// Leaning
	const FRGX_LeanInfo LeanInfo = CalculateLeanAmount();
	LeanAmount = UKismetMathLibrary::FInterpTo(LeanAmount, LeanInfo.LeanAmount, DeltaTime, LeanInfo.InterSpeed);
	// ------------------

	UKismetSystemLibrary::DrawDebugCircle(GetWorld(), GetActorLocation(), 100.0f, 24, FLinearColor::Green, 0.0f, 0.0f, FVector(0.0f, 1.0f, 0.0f), FVector(1.0f, 0.0f, 0.0f));
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

	UE_LOG(LogTemp, Warning, TEXT("On Follow Combo\n"));

	FGameplayTag NextAttack = ComboSystemComponent->GetNextAttack();
	if (NextAttack != FGameplayTag::RequestGameplayTag("Combo.None"))
	{
		FString NextAttackString = NextAttack.ToString();
		UE_LOG(LogTemp, Warning, TEXT("Next Attack: %s\n"), *NextAttackString);
		
		// Fire next attack
		FGameplayEventData EventData;
		int32 TriggeredAbilities = AbilitySystemComponent->HandleGameplayEvent(NextAttack, &EventData);

		UE_LOG(LogTemp, Warning, TEXT("Triggered Abilities: %d\n"), TriggeredAbilities);

		// Clear next attack status
		ComboSystemComponent->CleanStatus(TriggeredAbilities);
	}
}

void ARGX_PlayerCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f) && !bIgnoreInputMoveVector)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ARGX_PlayerCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f) && !bIgnoreInputMoveVector)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		// get right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ARGX_PlayerCharacter::TurnAtRate(float Rate)
{
	YawChange = Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds();
	AddControllerYawInput(YawChange);
}

void ARGX_PlayerCharacter::LookUpAtRate(float Rate)
{
	PitchChange = Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds();
	AddControllerPitchInput(PitchChange);
}

FRGX_LeanInfo ARGX_PlayerCharacter::CalculateLeanAmount()
{
	FRGX_LeanInfo LeanInfo;

	const float YawChangeClamped = UKismetMathLibrary::FClamp(YawChange, -1.0f, 1.0f);
	const bool bInsuficientVelocity = GetCharacterMovement()->IsFalling() || GetVelocity().Size() < 5.0f;

	if (bInsuficientVelocity == true)
	{
		LeanInfo.LeanAmount = 0.0f;
		LeanInfo.InterSpeed = 10.0f;
	}
	else
	{
		LeanInfo.LeanAmount = YawChangeClamped;
		LeanInfo.InterSpeed = 1.0f;
	}

	return LeanInfo;
}

void ARGX_PlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	ECollisionChannel CollisionChannel = Hit.GetComponent()->GetCollisionObjectType();
	if (CollisionChannel == ECollisionChannel::ECC_WorldStatic)
	{
		InputHandlerComponent->ResetAirState();

		AddGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.CanAirCombo")));
		RemoveGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.HasAirDashed")));
		bIsFallingDown = false;
	}
}

void ARGX_PlayerCharacter::OnCapsuleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//UE_LOG(LogTemp, Warning, TEXT("On Capsule Overlap\n"));

	if (bIsFallingDown == true)
	{
		const FVector Normal = Hit.Normal;
		const FVector PlayerLaunchForce = Normal * FVector(1.0f, 1.0f, -1.0f) * 100.0f;
		const FVector OtherActorLaunchForce = Normal * -1.0f * 2000.0f;

		LaunchCharacter(PlayerLaunchForce, true, true);

		ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(OtherActor);
		if (Enemy)
		{
			Enemy->LaunchCharacter(OtherActorLaunchForce, true, true);
		}
	}
}

void ARGX_PlayerCharacter::OnJump_Implementation()
{

}
