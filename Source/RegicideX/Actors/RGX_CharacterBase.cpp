
#include "RGX_CharacterBase.h"
#include "Components/MCV_AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayAbilitySpec.h"
#include "Camera/CameraShakeBase.h"
#include "Kismet/KismetMathLibrary.h"

ARGX_CharacterBase::ARGX_CharacterBase()
{
	AbilitySystemComponent	= CreateDefaultSubobject<UMCV_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet			= CreateDefaultSubobject<URGX_AttributeSet>(TEXT("AttributeSet"));

	CharacterLevel = 1;
	bAbilitiesInitialized = false;
}

void ARGX_CharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init abilities
	if (AbilitySystemComponent)
	{
		AddStartupGameplayAbilities();
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void ARGX_CharacterBase::UnPossessed()
{
}

void ARGX_CharacterBase::HandleEndKnockedUp()
{
	OnHandleEndKnockedUp();
}

void ARGX_CharacterBase::OnHandleEndKnockedUp()
{
	ResetGravity();
}

void ARGX_CharacterBase::ResetGravity()
{
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	if (CharacterMovementComponent)
	{
		CharacterMovementComponent->MaxAcceleration = MaxAcceleration;
		CharacterMovementComponent->GravityScale = GravityScale;
	}
}

void ARGX_CharacterBase::BeginPlay()
{
	Super::BeginPlay();

	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	if (CharacterMovementComponent)
	{
		CharacterMovementComponent->MaxWalkSpeed = MoveSpeed;
		CharacterMovementComponent->MaxAcceleration = MaxAcceleration;
		CharacterMovementComponent->GravityScale = GravityScale;
	}

}

void ARGX_CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bWasFallingDownThisFrame = IsFallingDown();

	CheckKnockUpState();
}

UAbilitySystemComponent* ARGX_CharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

bool ARGX_CharacterBase::IsFallingDown()
{
	return GetCharacterMovement() && GetCharacterMovement()->IsFalling() && GetVelocity().Z < 0;
}

float ARGX_CharacterBase::GetOriginalMaxSpeed() const
{
	return MoveSpeed;
}

float ARGX_CharacterBase::GetCurrentMaxSpeed() const
{
	return GetCharacterMovement()->MaxWalkSpeed;
}

void ARGX_CharacterBase::SetCurrentMaxSpeed(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

float ARGX_CharacterBase::GetOriginalMaxAcceleration() const
{
	return MaxAcceleration;
}

float ARGX_CharacterBase::GetCurrentMaxAcceleration() const
{
	return GetCharacterMovement()->MaxAcceleration;
}

void ARGX_CharacterBase::SetCurrentMaxAcceleration(float Acceleration)
{
	GetCharacterMovement()->MaxAcceleration = Acceleration;
}

float ARGX_CharacterBase::GetOriginalGravityScale() const
{
	return GravityScale;
}

float ARGX_CharacterBase::GetCurrentGravityScale() const
{
	return GetCharacterMovement()->GravityScale;
}

void ARGX_CharacterBase::SetCurrentGravityScale(float Scale)
{
	GetCharacterMovement()->GravityScale = Scale;
}

float ARGX_CharacterBase::GetHealth() const
{
	const URGX_AttributeSet* attrSet = GetAbilitySystemComponent()->GetSet<URGX_AttributeSet>();
	if (!attrSet)
		return 1.0f;
	return attrSet->GetHealth();
}

float ARGX_CharacterBase::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

int32 ARGX_CharacterBase::GetCharacterLevel() const
{
	return CharacterLevel;
}

bool ARGX_CharacterBase::SetCharacterLevel(int32 NewLevel)
{
	if (CharacterLevel != NewLevel && NewLevel > 0)
	{
		// Our level changed so we need to refresh abilities
		RemoveStartupGameplayAbilities();
		CharacterLevel = NewLevel;
		AddStartupGameplayAbilities();

		return true;
	}
	return false;
}

bool ARGX_CharacterBase::IsAlive()
{
	return GetHealth() > 0.0f ? true : false;
}

bool ARGX_CharacterBase::CanBeLaunched(AActor* ActorInstigator, URGX_LaunchEventDataAsset* LaunchPayload)
{
	return true;
}

void ARGX_CharacterBase::OnBeingLaunched(
	AActor* ActorInstigator,
	URGX_LaunchEventDataAsset* LaunchPayload)
{
	if (CanBeLaunched(ActorInstigator, LaunchPayload) == false) return;

	// Decompose payload
	const FVector ActorLocation = GetActorLocation();
	float VerticalForce = LaunchPayload->LaunchVerticalForce;
	const float HorizontalForce = LaunchPayload->LaunchHorizontalForce;
	const bool bOverrideXY = LaunchPayload->bOverrideHorizontal;
	const bool bOverrideZ = LaunchPayload->bOverrideVertical;

	FVector LaunchHorizontalDirection;

	// Get force horizontal direction depending on the instigator origin or if the actor applies a force to itself
	if (ActorInstigator && LaunchPayload->bInstigatorOrigin)
	{
		const FVector ForceOrigin = ActorInstigator->GetActorLocation();
		LaunchHorizontalDirection = ActorLocation - ForceOrigin;
		LaunchHorizontalDirection.Z = 0.0f;
		LaunchHorizontalDirection.Normalize();
	}
	else
	{
		LaunchHorizontalDirection = GetActorForwardVector();
	}

	if (bCanBeKnockup == false)
	{
		VerticalForce = 0.0f;
	}

	FVector LaunchForce = LaunchHorizontalDirection * HorizontalForce + FVector(0.0f, 0.0f, 1.0f) * VerticalForce;

	LaunchCharacter(LaunchForce, bOverrideXY, bOverrideZ);

	if (LaunchPayload->bKnockUp == true && bCanBeKnockup == true)
	{
		AddGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.KnockedUp")));
	}
	// TODO: If the character is in air maybe it is mandatory to apply a minimum Z force due to an Unreal bug
}

void ARGX_CharacterBase::RotateDirectlyTowardsActor(const AActor* Target, bool bFaceBackwards)
{
	const FRotator selfRotation = GetActorRotation();

	const FVector selfLocation = GetActorLocation();
	const FVector targetLocation = Target->GetActorLocation();
	const FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(selfLocation, targetLocation);

	const FRotator desiredRotation = FRotator(selfRotation.Pitch, bFaceBackwards ? lookRotation.Yaw + 180.0f : lookRotation.Yaw, selfRotation.Roll);

	SetActorRotation(desiredRotation);
}

void ARGX_CharacterBase::OnHitboxHit(UGameplayAbility* MeleeAbility, FGameplayEventData EventData, TSubclassOf<UCameraShakeBase> CameraShakeClass)
{
}

void ARGX_CharacterBase::HandleDamage(
	float DamageAmount, 
	const FHitResult& HitInfo, 
	const FGameplayTagContainer& DamageTags, 
	ARGX_CharacterBase* InstigatorCharacter, 
	AActor* DamageCauser,
	ERGX_AnimEvent HitReactFlag)
{
	// If it is already dead, return
	if (IsAlive() && HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.Dead"))))
	{
		return;
	}

	// Add the Status.Dead tag if not alive to avoid getting affected by Gameplay Effects.
	if(IsAlive() == false)
	{
		GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.Dead")));
	}

	OnDamaged(DamageAmount, HitInfo, DamageTags, InstigatorCharacter, DamageCauser);

	if (IsAlive())
	{
		AActor* damageSource = nullptr;
		if (DamageCauser)
		{
			damageSource = DamageCauser;
		}
		else if (InstigatorCharacter)
		{
			damageSource = InstigatorCharacter;
		}

		if (damageSource)
		{
			FVector Direction = GetActorLocation() - damageSource->GetActorLocation();
			HitReactDirection = Direction.GetSafeNormal2D();
			RotateDirectlyTowardsActor(damageSource, WasHitInTheBack());
		}
	}
}

void ARGX_CharacterBase::HandleHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags)
{
	OnHealthChanged(DeltaValue, EventTags);
}

float ARGX_CharacterBase::HandleDamageMitigation(float DamageAmount, const FHitResult& HitInfo, const FGameplayTagContainer& DamageTags, ARGX_CharacterBase* InstigatorCharacter, AActor* DamageCauser)
{
	return DamageAmount;
}

void ARGX_CharacterBase::NotifyDeath()
{
	OnHandleDeath();
}

void ARGX_CharacterBase::HandleDeath()
{
}

void ARGX_CharacterBase::AddStartupGameplayAbilities()
{
	if (AbilitySystemComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No ability system component when adding startup abilities."));
		return;
	}

	// Get granted abilities.
	for (const FRGX_AbilityInitData& StartupAbility : GameplayAbilities)
	{
		UClass* AbilityClass = StartupAbility.Ability.Get();
		if (AbilityClass)
		{
			FGameplayAbilitySpec AbilitySpec(AbilityClass, 1, static_cast<int32>(StartupAbility.InputID));
			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}
	}

	// Apply effects, such as initializing the base attributes.
	for (TSubclassOf<UGameplayEffect>& GameplayEffect : PassiveGameplayEffects)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActivateEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
		}
	}

	bAbilitiesInitialized = true;
}

void ARGX_CharacterBase::RemoveStartupGameplayAbilities()
{
	if (AbilitySystemComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No ability system component when removing startup abilities."));
		return;
	}

	// Remove any abilities added from a previous call
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		FRGX_AbilityInitData Ability;
		Ability.InputID = ERGX_AbilityInputID(Spec.InputID);
		if (Spec.SourceObject == this)
		{
			for (const auto& GA : GameplayAbilities)
			{
				if (GA.Ability == Spec.Ability->GetClass()) {
					AbilitiesToRemove.Add(Spec.Handle);
				}
			}
		}
	}

	// Do in two passes so the removal happens after we have the full list
	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	// Remove all of the passive gameplay effects that were applied by this character
	FGameplayEffectQuery Query;
	Query.EffectSource = this;
	AbilitySystemComponent->RemoveActiveEffects(Query);

	bAbilitiesInitialized = false;
}

FGenericTeamId ARGX_CharacterBase::GetGenericTeamId() const
{
	static const FGenericTeamId PlayerTeam(0);
	static const FGenericTeamId AITeam(1);
	return Cast<APlayerController>(GetController()) ? PlayerTeam : AITeam;
}

void ARGX_CharacterBase::CheckKnockUpState()
{
	FGameplayTag KnockUpTag = FGameplayTag::RequestGameplayTag("Status.KnockedUp");
	bool bWasKnockedUp = HasMatchingGameplayTag(KnockUpTag);
	if (bWasKnockedUp == true && IsFallingDown())
	{
		RemoveGameplayTag(KnockUpTag);

		UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
		if (CharacterMovementComponent)
		{
			// Change gravity scale
			CharacterMovementComponent->GravityScale = 0.0f;

			// Reset gravity scale after a delay
			FTimerDelegate TimerDel;
			FTimerHandle TimerHandle;
			TimerDel.BindUFunction(this, FName("HandleEndKnockedUp"));
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, TimeGravityZeroAfterKnockUp, false);
		}
	}
}

void ARGX_CharacterBase::AutoHurt()
{
}

void ARGX_CharacterBase::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	AbilitySystemComponent->GetOwnedGameplayTags(TagContainer);
}

bool ARGX_CharacterBase::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	return AbilitySystemComponent->HasMatchingGameplayTag(TagToCheck);
}

bool ARGX_CharacterBase::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystemComponent->HasAllMatchingGameplayTags(TagContainer);
}

bool ARGX_CharacterBase::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystemComponent->HasAnyMatchingGameplayTags(TagContainer);
}

void ARGX_CharacterBase::AddGameplayTag(const FGameplayTag& TagToAdd)
{
	AbilitySystemComponent->AddLooseGameplayTag(TagToAdd);
}

void ARGX_CharacterBase::RemoveGameplayTag(const FGameplayTag& TagToRemove, bool bCleanAll)
{
	if (bCleanAll)
	{
		const int32 TagCount = AbilitySystemComponent->GetTagCount(TagToRemove);
		AbilitySystemComponent->RemoveLooseGameplayTag(TagToRemove, TagCount);
	}
	else
	{
		AbilitySystemComponent->RemoveLooseGameplayTag(TagToRemove);
	}
}