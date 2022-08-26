// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_EnemyBase.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Components/MCV_AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "RegicideX/GameplayFramework/RGX_RoundGameMode.h"
#include "AbilitySystemGlobals.h"
#include "RegicideX/Components/RGX_HitboxesManagerComponent.h"
#include "RegicideX/UI/RGX_EnemyHealthBar.h"
#include "RegicideX/Components/RGX_InteractComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ARGX_EnemyBase::ARGX_EnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CombatTargetWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("CombatTargetWidgetComponent"));
	CombatTargetWidgetComponent->SetupAttachment(RootComponent);
	HealthDisplayWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthDisplayWidgetComponent"));
	HealthDisplayWidgetComponent->SetupAttachment(RootComponent);

	InteractionShapeComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionShapeComponent"));
	InteractionShapeComponent->SetupAttachment(RootComponent);

	DebugAttributesWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("DebugAttributesWidgetComponent"));
	DebugAttributesWidgetComponent->SetupAttachment(RootComponent);

	HitboxesManager = CreateDefaultSubobject<URGX_HitboxesManagerComponent>(TEXT("HitboxesManager"));

	bAbilitiesInitialized = false;
}

void ARGX_EnemyBase::Activate()
{
	Super::Activate();

	//HealthDisplayWidgetComponent->SetVisibility(true);
	//AddStartupGameplayAbilities();
}

void ARGX_EnemyBase::Deactivate()
{
	Super::Deactivate();
	GetMesh()->bPauseAnims = true;
	RemoveStartupGameplayAbilities();
}

// Called when the game starts or when spawned
void ARGX_EnemyBase::BeginPlay()
{
	Super::BeginPlay();

	InteractionShapeComponent->SetCollisionProfileName("InteractableObject");

	DisableInteraction();
	HideCombatTargetWidget();

	// For initializing health bar
	AddStartupGameplayAbilities();
	HandleHealthChanged(0.0f, FGameplayTagContainer());
}

void ARGX_EnemyBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AddGameplayTag(FGameplayTag::RequestGameplayTag("RGXCharacter.Enemy"));
}

void ARGX_EnemyBase::EraseRecentDamage(const float DamageAmount)
{
	RecentDamage -= DamageAmount;
}

void ARGX_EnemyBase::CheckIfWeak(float DamageAmount)
{
	// Execution Damage percentage
	RecentDamage += DamageAmount;

	FTimerDelegate TimerDel;
	FTimerHandle TimerHandle;
	TimerDel.BindUFunction(this, FName("EraseRecentDamage"), DamageAmount);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 2.f, false);

	const float MaxHealth = AbilitySystemComponent->GetNumericAttribute(AttributeSet->GetMaxHealthAttribute());
	const float CurrentHealth = AbilitySystemComponent->GetNumericAttribute(AttributeSet->GetHealthAttribute());
	const float RecentDamageAsHealthPercentage = RecentDamage / MaxHealth;
	const float HealthAsPercentage = CurrentHealth / MaxHealth;
	UE_LOG(LogTemp, Warning, TEXT("Percentage Recent Damage: %f\n"), RecentDamageAsHealthPercentage);
	if (/*RecentDamageAsHealthPercentage >= WeakenPercentage || */ HealthAsPercentage < WeakenPercentage)
	{
		if (CanBeInteractedWith(nullptr) == false)
			EnableInteraction();

		bWeak = true;
	}
}

void ARGX_EnemyBase::MoveToTarget(float DeltaTime, FVector TargetPos)
{
	if (TargetActor)
	{
		const FVector MyFront = this->GetActorForwardVector();
		const FVector CurrentLocation = this->GetActorLocation();
		FVector NewLocation = CurrentLocation + MyFront * MoveSpeed * DeltaTime;
		this->SetActorLocation(NewLocation);
	}
}

bool ARGX_EnemyBase::IsWeak()
{
	return bWeak;
}

void ARGX_EnemyBase::EnableInteraction()
{
	InteractionShapeComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UE_LOG(LogTemp, Warning, TEXT("Enable Interaction\n"));
}

void ARGX_EnemyBase::DisableInteraction()
{
	InteractionShapeComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UE_LOG(LogTemp, Warning, TEXT("Disable Interaction\n"));
}

void ARGX_EnemyBase::RotateToTarget(float DeltaTime)
{
	if (TargetActor)
	{
		const FVector MyLocation = this->GetActorLocation();
		const FVector TargetLocation = TargetActor->GetActorLocation();
		const FRotator RotOffset = UKismetMathLibrary::FindLookAtRotation(MyLocation, TargetLocation);
		FRotator NewRotation = FMath::Lerp(this->GetActorRotation(), RotOffset, DeltaTime * InterpSpeed);
		NewRotation.Pitch = 0.0;
		this->SetActorRotation(NewRotation);
	}
}

// Called every frame
void ARGX_EnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const bool bIsDead = HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.Dead")));
	if (bIsDead == true)
		return;

	if (TargetActor)
	{
		const FVector VectorToTarget = TargetActor->GetActorLocation() - GetActorLocation();
		const float DistanceToTarget = VectorToTarget.Size();
		if (DistanceToTarget > HealthBarHideDistance)
		{
			HealthDisplayWidgetComponent->SetVisibility(false);
		}
		else
		{
			HealthDisplayWidgetComponent->SetVisibility(true);
		}
	}
}

void ARGX_EnemyBase::HandleDamage(
	float DamageAmount,
	const FHitResult& HitInfo,
	const struct FGameplayTagContainer& DamageTags,
	ARGX_CharacterBase* InstigatorCharacter,
	AActor* DamageCauser)
{
	Super::HandleDamage(DamageAmount, HitInfo, DamageTags, InstigatorCharacter, DamageCauser);

	if (IsAlive())
	{
		CheckIfWeak(DamageAmount);

		if (IsWeak())
		{
			StopAnimMontage();
		}
		else
		{
			// Play reaction hit animation.
			if (GetMovementComponent()->IsFalling())
			{
				PlayAnimMontage(AMAirReactionHit);
			}
			else
			{
				PlayAnimMontage(AMReactionHit);
			}
		}
	}
	else
	{
		// If damage killed the actor, we should kill its AI Logic and clean weak status as it is already dead.
		bWeak = false;
		StopAnimMontage(); // If dead, make sure nothing is executing in order to execute death animation from AnimBP.
		AAIController* AiController = Cast<AAIController>(GetController());
		AiController->GetBrainComponent()->StopLogic(FString("Character dead."));
		PlayAnimMontage(AMDeath);
	}
}

void ARGX_EnemyBase::HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{
	Super::HandleHealthChanged(DeltaValue, EventTags);

	UMCV_AbilitySystemComponent* ACS	= Cast<UMCV_AbilitySystemComponent>(AbilitySystemComponent);
	URGX_EnemyHealthBar* HealthBar		= Cast<URGX_EnemyHealthBar>(HealthDisplayWidgetComponent->GetWidget());
	if (HealthBar)
	{
		HealthBar->MaxHealth		= ACS->GetNumericAttribute(AttributeSet->GetMaxHealthAttribute());
		HealthBar->CurrentHealth	= ACS->GetNumericAttribute(AttributeSet->GetHealthAttribute());
	}

	// Only call BP event if ACS is initialized
	if (ACS->bIsInitialized == true)
	{
		OnHealthChanged(DeltaValue, EventTags);
	}
}

void ARGX_EnemyBase::HandleDeath()
{
	Super::HandleDeath();

	OnHandleDeathEvent.Broadcast(ScoreValue);
	OnHandleDeath();
	HealthDisplayWidgetComponent->SetVisibility(false);
	//Deactivate();
	Destroy();
}

void ARGX_EnemyBase::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	CharacterTeam = TeamID;
}

FGenericTeamId ARGX_EnemyBase::GetGenericTeamId() const
{
	return CharacterTeam;
}

void ARGX_EnemyBase::ShowCombatTargetWidget()
{
	CombatTargetWidgetComponent->SetVisibility(true);
}

void ARGX_EnemyBase::HideCombatTargetWidget()
{
	CombatTargetWidgetComponent->SetVisibility(false);
}

void ARGX_EnemyBase::Interact(AActor* ActorInteracting)
{
	//UE_LOG(LogTemp, Warning, TEXT("Interaction With Enemy\n"));
	UAbilitySystemComponent* ACS = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(ActorInteracting); 

	if (ACS)
	{
		FGameplayEventData EventData;
		EventData.Instigator = ActorInteracting;
		EventData.Target = this;
		ACS->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Action.Execution.Ability")), &EventData);
	}
}

void ARGX_EnemyBase::StartCanInteract(AActor* ActorInteracting)
{
	URGX_InteractComponent* InteractComponent = ActorInteracting->FindComponentByClass<URGX_InteractComponent>();
	FString Text = "Execute";
	InteractComponent->SetInteractionText(FText::FromString(Text));
	InteractComponent->ShowInteractWidget();
}

void ARGX_EnemyBase::StopCanInteract(AActor* ActorInteracting)
{
	URGX_InteractComponent* InteractComponent = ActorInteracting->FindComponentByClass<URGX_InteractComponent>();
	InteractComponent->HideInteractWidget();
}

bool ARGX_EnemyBase::CanBeInteractedWith(AActor* ActorInteracting)
{
	//UE_LOG(LogTemp, Warning, TEXT("Can be interacted with\n"));
	return InteractionShapeComponent->IsCollisionEnabled();
}

bool ARGX_EnemyBase::IsInFrustum()
{
	ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (LocalPlayer != nullptr && LocalPlayer->ViewportClient != nullptr && LocalPlayer->ViewportClient->Viewport)
	{
		FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(
			LocalPlayer->ViewportClient->Viewport,
			GetWorld()->Scene,
			LocalPlayer->ViewportClient->EngineShowFlags)
			.SetRealtimeUpdate(true));

		FVector ViewLocation;
		FRotator ViewRotation;
		FSceneView* SceneView = LocalPlayer->CalcSceneView(&ViewFamily, ViewLocation, ViewRotation, LocalPlayer->ViewportClient->Viewport);
		if (SceneView != nullptr)
		{
			return SceneView->ViewFrustum.IntersectSphere(
				GetActorLocation(), GetSimpleCollisionRadius());
		}
	}

	return false;
}

void ARGX_EnemyBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ARGX_RoundGameMode* MyGameMode = Cast<ARGX_RoundGameMode>(GetWorld()->GetAuthGameMode()))
	{
		//MyGameMode->OnEnemyDeath(0);
	}
	Super::EndPlay(EndPlayReason);
}
