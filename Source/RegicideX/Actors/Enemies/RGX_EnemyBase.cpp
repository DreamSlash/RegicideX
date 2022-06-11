// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_EnemyBase.h"
#include "Components/MCV_AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "RegicideX/GAS/AttributeSets/RGX_HealthAttributeSet.h"
#include "Kismet/KismetMathLibrary.h"
#include "RegicideX/GameplayFramework/RGX_RoundGameMode.h"
#include "AbilitySystemGlobals.h"
#include "RegicideX/Components/RGX_HitboxesManagerComponent.h"
#include "RegicideX/UI/RGX_EnemyHealthBar.h"

// Sets default values
ARGX_EnemyBase::ARGX_EnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CombatTargetWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("CombatTargetWidgetComponent"));
	CombatTargetWidgetComponent->SetupAttachment(RootComponent);
	HealthDisplayWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthDisplayWidgetComponent"));
	HealthDisplayWidgetComponent->SetupAttachment(RootComponent);

	AbilitySystemComponent = CreateDefaultSubobject<UMCV_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	InteractionShapeComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionShapeComponent"));
	InteractionShapeComponent->SetupAttachment(RootComponent);

	HealthAttributeSet = CreateDefaultSubobject<URGX_HealthAttributeSet>(TEXT("HealthAttributeSet"));

	DebugAttributesWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("DebugAttributesWidgetComponent"));
	DebugAttributesWidgetComponent->SetupAttachment(RootComponent);

	HitboxesManager = CreateDefaultSubobject<URGX_HitboxesManagerComponent>(TEXT("HitboxesManager"));
}

// Called when the game starts or when spawned
void ARGX_EnemyBase::BeginPlay()
{
	Super::BeginPlay();

	InteractionShapeComponent->SetCollisionProfileName("InteractableObject");

	DisableInteraction();

	// For initializing health bar
	HandleHealthChanged(0.0f);
}

void ARGX_EnemyBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AddGameplayTag(FGameplayTag::RequestGameplayTag("RGXCharacter.Enemy"));
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
}

// Called to bind functionality to input
void ARGX_EnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* ARGX_EnemyBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ARGX_EnemyBase::HandleDamage(FAttackInfo info)
{
}

void ARGX_EnemyBase::HandleDamage(float DamageAmount, AActor* DamageCauser)
{
	OnHandleDamage(DamageAmount, DamageCauser);
}

void ARGX_EnemyBase::HandleHealthChanged(float DeltaValue)
{
	UMCV_AbilitySystemComponent* ACS = Cast<UMCV_AbilitySystemComponent>(AbilitySystemComponent);
	URGX_EnemyHealthBar* HealthBar = Cast<URGX_EnemyHealthBar>(HealthDisplayWidgetComponent->GetWidget());
	if (HealthBar)
	{
		HealthBar->MaxHealth = ACS->GetNumericAttribute(HealthAttributeSet->GetMaxHealthAttribute());
		HealthBar->CurrentHealth = ACS->GetNumericAttribute(HealthAttributeSet->GetHealthAttribute());
	}

	// Only call BP event if ACS is initialized
	if (ACS->bIsInitialized == true)
	{
		OnHandleHealthChanged(DeltaValue);
	}
}

void ARGX_EnemyBase::HandleDeath()
{
	OnHandleDeath();
}

void ARGX_EnemyBase::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	CharacterTeam = TeamID;
}

FGenericTeamId ARGX_EnemyBase::GetGenericTeamId() const
{
	return CharacterTeam;
}

void ARGX_EnemyBase::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	AbilitySystemComponent->GetOwnedGameplayTags(TagContainer);
}

bool ARGX_EnemyBase::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	return AbilitySystemComponent->HasMatchingGameplayTag(TagToCheck);
}

bool ARGX_EnemyBase::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystemComponent->HasAllMatchingGameplayTags(TagContainer);
}

bool ARGX_EnemyBase::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystemComponent->HasAnyMatchingGameplayTags(TagContainer);
}

void ARGX_EnemyBase::AddGameplayTag(const FGameplayTag& TagToAdd)
{
	AbilitySystemComponent->AddLooseGameplayTag(TagToAdd);
}

void ARGX_EnemyBase::RemoveGameplayTag(const FGameplayTag& TagToRemove)
{
	AbilitySystemComponent->RemoveLooseGameplayTag(TagToRemove);
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
	// TODO: Show Widget
}

void ARGX_EnemyBase::StopCanInteract(AActor* ActorInteracting)
{
	// TODO: Hide Widget
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
