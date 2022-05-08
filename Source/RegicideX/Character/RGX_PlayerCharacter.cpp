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
#include "../Actors/Enemies/RGX_EnemyBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

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
	PlayerInputComponent->BindAction("SwitchPowerSkill", IE_Pressed, this, &ARGX_PlayerCharacter::ChangePowerSkill);
	PlayerInputComponent->BindAction("TimeScale", IE_Pressed, this, &ARGX_PlayerCharacter::ChangeTimeScale);

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
	FGameplayTag NextAttack = ComboSystemComponent->ManageInputToken(ERGXPlayerInputID::LightAttackInput);

	if (NextAttack == FGameplayTag::RequestGameplayTag(FName("Combo.Light")))
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
}

void ARGX_PlayerCharacter::ManageHeavyAttackInput()
{
	FGameplayTag NextAttack = ComboSystemComponent->ManageInputToken(ERGXPlayerInputID::HeavyAttackInput);

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

// TODO [REFACTOR]: Component for autoassist
void ARGX_PlayerCharacter::PerformAttackAutoAssist()
{
	FVector PlayerLocation = GetActorLocation();

	float radius = 300.0f;

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	UClass* SeekClass = ARGX_EnemyBase::StaticClass();

	TArray<AActor*> IgnoreActors;
	TArray<AActor*> OutActors;

	// Check for nearby enemies
	if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), PlayerLocation, radius, TraceObjectTypes, SeekClass, IgnoreActors, OutActors) == false)
		return;

	float CurrentClosestDistance = INFINITY;
	FVector NearestEnemyLocation = FVector(0.0f, 0.0f, 0.0f);
	bool bHasTarget = false;

	// Check the closest enemy inside a cone in front of the player
	for (AActor* Actor : OutActors)
	{
		ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(Actor);

		bool bIsDead = Enemy->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.Dead")));

		if (bIsDead == true)
			continue;

		FVector EnemyLocation = Enemy->GetActorLocation();

		float Distance = FVector::Dist(PlayerLocation, EnemyLocation);
		
		FVector PlayerToEnemyVector = EnemyLocation - PlayerLocation;
		PlayerToEnemyVector.Normalize();

		FVector PlayerForward = GetActorForwardVector();

		// Cone check
		float Dot = FVector::DotProduct(PlayerToEnemyVector, PlayerForward);

		if (Distance < CurrentClosestDistance && Dot > 0.5f)
		{
			CurrentClosestDistance = Distance;
			NearestEnemyLocation = EnemyLocation;
			bHasTarget = true;
		}
	}

	if (bHasTarget == false)
		return;

	FVector PlayerToEnemyVector = NearestEnemyLocation - PlayerLocation;
	FRotator Rotation = UKismetMathLibrary::MakeRotFromX(PlayerToEnemyVector);

	SetActorRotation(Rotation);

	float OffsetToEnemy = 150.0f;

	if (CurrentClosestDistance > OffsetToEnemy == false)
		return;

	FVector AssistDirection = FVector(PlayerToEnemyVector.X, PlayerToEnemyVector.Y, 0.0f);
	AssistDirection.Normalize();

	FVector FinalLocation = PlayerLocation + AssistDirection * (CurrentClosestDistance - OffsetToEnemy);

	SetActorLocation(FinalLocation);
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

	const bool bIsBeingAttacked = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(), PlayerLocation, 200.0f, TraceObjectTypes, nullptr, IgnoreActors, OutActors
	);

	return OutActors.Num() > 0;
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

	if (PowerSkills.Num() < 1 == false)
	{
		CurrentSkillTag = PowerSkills[0];
		AddGameplayTag(CurrentSkillTag);
	}
}

void ARGX_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Leaning
	FRGX_LeanInfo LeanInfo = CalculateLeanAmount();
	LeanAmount = UKismetMathLibrary::FInterpTo(LeanAmount, LeanInfo.LeanAmount, DeltaTime, LeanInfo.InterSpeed);
	// ------------------

	if (IsBeingAttacked())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Being Attacked\n"));
	}

	UKismetSystemLibrary::DrawDebugCircle(GetWorld(), GetActorLocation(), 200.0f, 24, FLinearColor::Green, 0.0f, 0.0f, FVector(0.0f, 1.0f, 0.0f), FVector(1.0f, 0.0f, 0.0f));
}

UAbilitySystemComponent* ARGX_PlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ARGX_PlayerCharacter::AddMovementVector(FVector MovementVector)
{
	MoveVector = MovementVector;
	bAddMoveVector = true;
}

void ARGX_PlayerCharacter::RemoveMovementVector()
{
	MoveVector = FVector(0.0f);
	bAddMoveVector = false;
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

void ARGX_PlayerCharacter::OnJump_Implementation()
{

}
