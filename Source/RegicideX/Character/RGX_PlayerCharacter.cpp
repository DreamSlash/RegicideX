#include "RGX_PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GenericTeamAgentInterface.h"
#include "Components/InputComponent.h"
#include "../GameplayFramework/RGX_PlayerState.h" // TODO: write path to project settings
#include "../Components/RGX_ComboSystemComponent.h"
#include "../GAS/AttributeSets/RGX_HealthAttributeSet.h"
#include "../GAS/AttributeSets/RGX_CombatAttributeSet.h"

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

	AbilitySystemComponent = CreateDefaultSubobject<UMCV_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	ComboSystemComponent = CreateDefaultSubobject<URGX_ComboSystemComponent>(TEXT("ComboSystemComponent"));
	HealthAttributeSet = CreateDefaultSubobject<URGX_HealthAttributeSet>(TEXT("HealthAttributeSet"));
	CombatAttributeSet = CreateDefaultSubobject<URGX_CombatAttributeSet>(TEXT("CombatAttributeSet"));
}

void ARGX_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Debug", IE_Pressed, this, &ARGX_PlayerCharacter::PrintDebugInformation);

	PlayerInputComponent->BindAction("LightAttack", IE_Pressed, this, &ARGX_PlayerCharacter::ManageLightAttackInput);
	PlayerInputComponent->BindAction("HeavyAttack", IE_Pressed, this, &ARGX_PlayerCharacter::ManageHeavyAttackInput);
	//PlayerInputComponent->BindAction("PowerSkill", IE_Pressed, this, &ARGX_PlayerCharacter::ManagePowerSkillInput);

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
	//AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbilityInputBinds(FString("PowerSkill"), FString("CancelPowerSkill"), FString("EMCVAbilityInputID"), static_cast<int32>(EMCVAbilityInputID::PowerSkill), static_cast<int32>(EMCVAbilityInputID::CancelPowerSkill)));
	//AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbilityInputBinds(FString("Jump"), FString("StopJump"), FString("EMCVAbilityInputID"), static_cast<int32>(EMCVAbilityInputID::Jump), static_cast<int32>(EMCVAbilityInputID::StopJump)));
	//AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbilityInputBinds(FString("Evasion"), FString("StopEvasion"), FString("EMCVAbilityInputID"), static_cast<int32>(EMCVAbilityInputID::Evasion), static_cast<int32>(EMCVAbilityInputID::CancelEvasion)));
	//AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbilityInputBinds(FString("LightAttack"), FString("CancelLightAttack"), FString("EMCVAbilityInputID"), static_cast<int32>(EMCVAbilityInputID::LightAttack), static_cast<int32>(EMCVAbilityInputID::CancelLightAttack)));
	//AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbilityInputBinds(FString("HeavyAttack"), FString("CancelHeavyAttack"), FString("EMCVAbilityInputID"), static_cast<int32>(EMCVAbilityInputID::HeavyAttack), static_cast<int32>(EMCVAbilityInputID::CancelHeavyAttack)));
}

void ARGX_PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
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
	ComboSystemComponent->ManageInputToken(ERGXPlayerInputID::LightAttackInput);
}

void ARGX_PlayerCharacter::ManageHeavyAttackInput()
{
	ComboSystemComponent->ManageInputToken(ERGXPlayerInputID::HeavyAttackInput);
}

void ARGX_PlayerCharacter::ManagePowerSkillInput()
{
	// TODO: Make a component for managing skills?

	if (PowerSkills.Num() <= CurrentSkillSelected)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Manuela\n"));
	// Fire next attack
	FGameplayEventData EventData;
	AbilitySystemComponent->HandleGameplayEvent(PowerSkills[CurrentSkillSelected], &EventData);
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
}

void ARGX_PlayerCharacter::PrintDebugInformation()
{
	TArray<FGameplayAttribute> attributes;
	AbilitySystemComponent->GetAllAttributes(attributes);

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

	if (PowerSkills.Num() < 1 == false)
	{
		CurrentSkillTag = PowerSkills[0];
		AddGameplayTag(CurrentSkillTag);
	}
}

void ARGX_PlayerCharacter::Tick(float DeltaTime)
{
	// Combo system
	FGameplayTag NextAttack = ComboSystemComponent->GetNextAttack();
	if (NextAttack != FGameplayTag::RequestGameplayTag("Combo.None"))
	{
		// Call AbilityComponentSystem
		FString NextAttackString = NextAttack.ToString();
		UE_LOG(LogTemp, Warning, TEXT("Next Attack: %s\n"), *NextAttackString);

		// Fire next attack
		FGameplayEventData EventData;
		AbilitySystemComponent->HandleGameplayEvent(NextAttack, &EventData);

		// Clear next attack status
		ComboSystemComponent->CleanNextAttack();
	}
	// --------------------
}

UAbilitySystemComponent* ARGX_PlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ARGX_PlayerCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
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
	if ((Controller != nullptr) && (Value != 0.0f))
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
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARGX_PlayerCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}