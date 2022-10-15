// Fill out your copyright notice in the Description page of Project Settings.

#include "RGX_EnemyBase.h"

#include "AbilitySystemGlobals.h"
#include "Animation/AnimInstance.h"
#include "BrainComponent.h"
#include "Components/MCV_AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "RegicideX/AI/Controllers/RGX_EnemyBaseController.h"
#include "RegicideX/GameplayFramework/RGX_RoundGameMode.h"
#include "RegicideX/Components/RGX_HitboxesManagerComponent.h"
#include "RegicideX/UI/RGX_EnemyHealthBar.h"
#include "RegicideX/Components/RGX_InteractComponent.h"

// Sets default values
ARGX_EnemyBase::ARGX_EnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CombatTargetWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("CombatTargetWidgetComponent"));
	CombatTargetWidgetComponent->SetupAttachment(RootComponent);

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
	//AddStartupGameplayAbilities();
}

void ARGX_EnemyBase::Deactivate()
{
	Super::Deactivate();
	GetMesh()->bPauseAnims = true;
	RemoveStartupGameplayAbilities();
}

ERGX_EnemyType ARGX_EnemyBase::GetEnemyType() const
{
	return EnemyType;
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

	//TargetActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
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
	if (HealthAsPercentage < WeakenPercentage)
	{/*
		if (CanBeInteractedWith_Implementation(nullptr) == false)
			EnableInteraction();

		bWeak = true;

		if (HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Status.Enemy.Weakened")) == false)
		{
			AddGameplayTag(FGameplayTag::RequestGameplayTag("Status.Enemy.Weakened"));
		}*/
	}
}

void ARGX_EnemyBase::MoveToTarget(float DeltaTime, FVector TargetPos)
{
	if (TargetActor)
	{
		const FVector MyFront = this->GetActorForwardVector();
		const FVector CurrentLocation = this->GetActorLocation();
		FVector NewLocation = CurrentLocation + MyFront * GetCurrentMaxSpeed() * DeltaTime;
		this->SetActorLocation(NewLocation);
	}
}

void ARGX_EnemyBase::StopLogic(const FString& Reason)
{
	AAIController* AiController = Cast<AAIController>(GetController());
	if (AiController)
	{
		UBrainComponent* BrainComponent = AiController->GetBrainComponent();
		if (BrainComponent)
		{
			BrainComponent->StopLogic(Reason);
		}
	}
}

bool ARGX_EnemyBase::IsWeak()
{
	return bWeak;
}

void ARGX_EnemyBase::EnableInteraction()
{
	InteractionShapeComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//UE_LOG(LogTemp, Warning, TEXT("Enable Interaction\n"));
}

void ARGX_EnemyBase::DisableInteraction()
{
	InteractionShapeComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//UE_LOG(LogTemp, Warning, TEXT("Disable Interaction\n"));
}

void ARGX_EnemyBase::RotateTowardsTarget()
{
	FOnTimelineFloat TimelineCallback;
	/*FOnTimelineEventStatic TimelineFinishedCallback;
	TimelineFinishedCallback.BindLambda([this]()
		{
			GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, [this]() {Destroy(); }, LifeTimeOnceIsRisen, false);
		});*/

	RotationTowardsTargetTimeline.AddInterpFloat(RotationTowardsTargetCurve, TimelineCallback);
	//RiseTimeLine.SetTimelineFinishedFunc(TimelineFinishedCallback);

	RotationTowardsTargetTimeline.PlayFromStart();
}

void ARGX_EnemyBase::StopRotatingTowardsEnemy()
{
	RotationTowardsTargetTimeline.Stop();
}

void ARGX_EnemyBase::RotateToTarget(float DeltaTime)
{
	if (TargetActor)
	{
		const float InterpSpeed = HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("GameplayEvent.Action.Melee")) ? AttackRotationInterpSpeed : RotationInterpSpeed;
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

	if (IsAlive() == false)
		return;

	if (TargetActor)
	{
		/*if (bHasLostSightOfPlayer)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				const float CurrentTime = World->GetTimeSeconds();
				if (CurrentTime - TimeLostSight > TimeBeforeTurningInPlace && bCanRotate && bDefaultFocusPlayer)
				{
					RotateToTarget(DeltaTime);
				}
			}
		}
		else */if (bCanRotate && bDefaultFocusPlayer)
		{
			RotateToTarget(DeltaTime);
		}

		const FVector VectorToTarget = TargetActor->GetActorLocation() - GetActorLocation();

		//CheckIfHasLostSightOfPlayer();

		if (RotationTowardsTargetTimeline.IsPlaying())
		{
			RotationTowardsTargetTimeline.TickTimeline(DeltaTime);

			const float alpha = RotationTowardsTargetCurve->GetFloatValue(RotationTowardsTargetTimeline.GetPlaybackPosition());

			const FRotator selfRotation = GetActorRotation();

			const FVector selfLocation = GetActorLocation();
			const FVector targetLocation = TargetActor->GetActorLocation();
			const FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(selfLocation, targetLocation);

			const FRotator desiredRotation = FRotator(selfRotation.Pitch, lookRotation.Yaw, selfRotation.Roll);

			const FRotator finalRotation = FMath::Lerp(selfRotation, desiredRotation, alpha);

			SetActorRotation(finalRotation);
		}
	}
}

void ARGX_EnemyBase::CheckIfHasLostSightOfPlayer()
{
	const FVector MyForward = GetActorForwardVector();
	const FVector VectorToTarget = TargetActor->GetActorLocation() - GetActorLocation();
	const float Dot = FVector::DotProduct(MyForward, VectorToTarget);

	if (bHasLostSightOfPlayer == false)
	{
		if (TurnInPlaceCos > Dot)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				UE_LOG(LogTemp, Warning, TEXT("Has Lost Sight"));
				TimeLostSight = World->GetTimeSeconds();
				GetCharacterMovement()->MaxWalkSpeed = 0.0f;
				bHasLostSightOfPlayer = true;
			}
		}
	}
	else
	{
		if (RegainSightCos < Dot)
		{
			UE_LOG(LogTemp, Warning, TEXT("Has Regained Sight"));
			GetCharacterMovement()->MaxWalkSpeed = GetCurrentMaxSpeed();
			bHasLostSightOfPlayer = false;
		}
	}
}

void ARGX_EnemyBase::HandleDamage(
	float DamageAmount,
	const FHitResult& HitInfo,
	const struct FGameplayTagContainer& DamageTags,
	ARGX_CharacterBase* InstigatorCharacter,
	AActor* DamageCauser,
	ERGX_AnimEvent HitReactFlag)
{
	if (bImmune)
		return;

	Super::HandleDamage(DamageAmount, HitInfo, DamageTags, InstigatorCharacter, DamageCauser, HitReactFlag);

	FVector MyForward = GetActorForwardVector();
	MyForward.Z = 0.0f;
	MyForward.Normalize();

	FVector ToTarget = TargetActor->GetActorLocation() - GetActorLocation();
	ToTarget.Z = 0.0f;
	ToTarget.Normalize();

	const float DotProduct = FVector::DotProduct(MyForward, ToTarget);

	StopAnimMontage();
	if (IsAlive())
	{
		// Play reaction hit animation.
		if (GetMovementComponent()->IsFalling())
		{
			UAnimMontage* AnimToPlay = nullptr;
			const FAnimationArray AnimationList = *AnimMontageMap.Find(ERGX_AnimEvent::AirHitReact);
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
			UE_LOG(LogTemp, Warning, TEXT("Air Hit react"));
			PlayAnimMontage(AnimToPlay);
		}
		else
		{
			if(HitReactFlag == ERGX_AnimEvent::BasicHitReact)
				CheckIfWeak(DamageAmount);

			if (IsWeak() == false )
			{
				const FAnimationArray AnimationList = *AnimMontageMap.Find(HitReactFlag);
				UAnimMontage* AnimToPlay = nullptr;
				if (AnimationList.Animations.Num() > 0)
				{
					if (DotProduct < 0.0 && AnimationList.Animations.Num() > 2)
					{
						AnimToPlay = AnimationList.Animations[2];
					}
					else 
					{
						int32 Index = UKismetMathLibrary::RandomIntegerInRange(0, AnimationList.Animations.Num() - 2);
						AnimToPlay = AnimationList.Animations[Index];
					}
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Not animations assigned"));
					return;
				}

				PlayAnimMontage(AnimToPlay);
			}
		}
	}
	else
	{
		// If damage killed the actor, we should kill its AI Logic and clean weak status as it is already dead.
		bWeak = false;
		RemoveGameplayTag(FGameplayTag::RequestGameplayTag("Status.Enemy.Weakened"));
		//StopLogic("Character Dead");
		UAnimMontage* AnimToPlay = nullptr;
		const FAnimationArray AnimationList = *AnimMontageMap.Find(ERGX_AnimEvent::Death);
		if (AnimationList.Animations.Num() > 0)
		{
			if (DotProduct < 0.0 && AnimationList.Animations.Num() > 1) 
			{
				AnimToPlay = AnimationList.Animations[1];
			}
			else 
			{
				//int32 Index = UKismetMathLibrary::RandomIntegerInRange(0, AnimationList.Animations.Num() - 1);
				AnimToPlay = AnimationList.Animations[0];
			}
			
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Not animations assigned"));
			return;
		}
		PlayAnimMontage(AnimToPlay);
	}
}

void ARGX_EnemyBase::HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{
	Super::HandleHealthChanged(DeltaValue, EventTags);

	UMCV_AbilitySystemComponent* ACS	= Cast<UMCV_AbilitySystemComponent>(AbilitySystemComponent);
	// Only call BP event if ACS is initialized
	if (ACS->bIsInitialized == true)
	{
		OnHealthChanged(DeltaValue, EventTags);
	}
}

void ARGX_EnemyBase::HandleDeath()
{
	Super::HandleDeath();

	// TODO Make it random
	const int Quantity = FMath::RandRange(3.0f, 5.0f);
	SpawnSouls(Quantity);

	UE_LOG(LogTemp, Log, TEXT("Entering HandleDeath()"));
	if (OnHandleDeathEvent.IsBound())
	{
		OnHandleDeathEvent.Broadcast(this);
	}

	UE_LOG(LogTemp, Log, TEXT("Destroying actor..."));
	Destroy();
}

void ARGX_EnemyBase::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	CharacterTeam = TeamID;
}

void ARGX_EnemyBase::SpawnSouls(const int Quantity)
{
	// Create box
	const FVector ActorLocation = GetActorLocation();

	// Spawn n quantity of souls at random points
	for (int i = 0; i < Quantity; i++)
	{
		const FVector Location = UKismetMathLibrary::RandomPointInBoundingBox(ActorLocation, FVector(40.0f));
		const FRotator Rotation = FRotator(0.0f);
		GetWorld()->SpawnActor<AActor>(SoulParticleActor, ActorLocation, Rotation);
	}
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

void ARGX_EnemyBase::Interact_Implementation(AActor* ActorInteracting)
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

void ARGX_EnemyBase::StartCanInteract_Implementation(AActor* ActorInteracting)
{
	URGX_InteractComponent* InteractComponent = ActorInteracting->FindComponentByClass<URGX_InteractComponent>();
	FString Text = "Execute";
	InteractComponent->SetInteractionText(FText::FromString(Text));
	InteractComponent->ShowInteractWidget();
}

void ARGX_EnemyBase::StopCanInteract_Implementation(AActor* ActorInteracting)
{
	URGX_InteractComponent* InteractComponent = ActorInteracting->FindComponentByClass<URGX_InteractComponent>();
	InteractComponent->HideInteractWidget();
}

bool ARGX_EnemyBase::CanBeInteractedWith_Implementation(AActor* ActorInteracting)
{
	//UE_LOG(LogTemp, Warning, TEXT("Can be interacted with\n"));
	return InteractionShapeComponent->IsCollisionEnabled() && IsAlive();
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
