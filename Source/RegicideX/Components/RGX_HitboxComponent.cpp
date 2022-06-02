#include "RGX_HitboxComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "Components/ChildActorComponent.h"
#include "RegicideX/GameplayFramework/RGX_RoundGameMode.h"
#include "GenericTeamAgentInterface.h"
#include "RegicideX/GAS/RGX_PayloadObjects.h"
#include "RegicideX/Character/RGX_PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

URGX_HitboxComponent::URGX_HitboxComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URGX_HitboxComponent::BeginPlay()
{
	Super::BeginPlay();

	// Get references to all children colliders (colliders that represent the hitbox)
	TArray<USceneComponent*> Children;
	GetChildrenComponents(false, Children);
	for (USceneComponent* Child : Children)
	{
		UShapeComponent* ShapeComponent = Cast<UShapeComponent>(Child);

		if (ShapeComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("Collider Added to Hitbox\n"));
			Shapes.Add(ShapeComponent);
		}
	}

	for (UShapeComponent* shape : Shapes)
	{
		shape->OnComponentBeginOverlap.AddDynamic(this, &URGX_HitboxComponent::OnComponentOverlap);
	}

	if (bStartActive)
	{
		ActivateHitbox();
		ActivateEffect();
	}
	else
	{
		DeactivateHitbox();
		DeactivateEffect();
	}
}

void URGX_HitboxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (bIsStatic)
		return;

	// Tick component to get last position of socket for being able to calculate velocity direction later
	const ECollisionEnabled::Type CollisionType = GetCollisionEnabled();
	if (CollisionType == ECollisionEnabled::NoCollision)
		return;

	if (ChildActorComponent == nullptr)
		return;

	LastSocketPosition = ChildActorComponent->GetSocketLocation(SocketName);
}

void URGX_HitboxComponent::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	for (UShapeComponent* shape : Shapes)
	{
		shape->OnComponentBeginOverlap.RemoveDynamic(this, &URGX_HitboxComponent::OnComponentOverlap);
	}
}

void URGX_HitboxComponent::ActivateHitbox()
{
	const USceneComponent* Parent = GetAttachParent();
	AActor* OwnerActor = Parent->GetAttachmentRootActor();

	if (!OwnerActor)
	{
		OwnerActor = GetOwner();
	}

	//UE_LOG(LogTemp, Warning, TEXT("Activate Hitbox\n"));
	for (UShapeComponent* Shape : Shapes)
	{
		Shape->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Shape->SetCollisionProfileName("Dodgeable");
	}
}

void URGX_HitboxComponent::DeactivateHitbox()
{
	//UE_LOG(LogTemp, Warning, TEXT("Deactivate Hitbox\n"));
	for (UShapeComponent* Shape : Shapes)
	{
		Shape->SetCollisionProfileName("Dodgeable");
		Shape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	ActorsHit.Empty();
}

void URGX_HitboxComponent::ActivateEffect()
{
	//UE_LOG(LogTemp, Warning, TEXT("Activate Effect\n"));
	bEffectActivated = true;
	for (UShapeComponent* Shape : Shapes)
	{
		Shape->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Shape->SetCollisionProfileName("Dodgeable");
	}
}

void URGX_HitboxComponent::DeactivateEffect()
{
	//UE_LOG(LogTemp, Warning, TEXT("Deactivate Effect\n"));
	bEffectActivated = false;
	for (UShapeComponent* Shape : Shapes)
	{
		Shape->SetCollisionProfileName("Dodgeable");
		Shape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	ActorsHit.Empty();
}

void URGX_HitboxComponent::SetAbilityEffectsInfo(const FRGX_AbilityEffectsInfo& NewAbilityEffectsInfo)
{
	AbilityEffectsInfo = NewAbilityEffectsInfo;
}

void URGX_HitboxComponent::RemoveAbilityEffectsInfo()
{
	//UE_LOG(LogTemp, Warning, TEXT("Remove Ability Effects\n"));
	AbilityEffectsInfo.GameplayEffectsToTarget.Empty();
	AbilityEffectsInfo.GameplayEventsToTarget.Empty();
	AbilityEffectsInfo.GameplayEffectsToOwner.Empty();
	AbilityEffectsInfo.GameplayEventsToOwner.Empty();
}

// TODO [REFACTOR]: This functions should take into account the multiple shapes this class can have
bool URGX_HitboxComponent::IsGoingToOverlapActor(AActor* Actor)
{
	// if it is static we do not check if will overlap in the future, because it does not move
	if (bIsStatic)
		return false;

	const USceneComponent* Parent = GetAttachParent();
	AActor* OwnerActor = Parent->GetAttachmentRootActor();

	FVector Direction = FVector(0.0f);
	FVector StartLocation = FVector(0.0f);

	// means it is not an actor spawned by a child actor
	if (!OwnerActor)
	{
		OwnerActor = GetOwner();

		Direction = OwnerActor->GetVelocity();
		Direction.Normalize();

		StartLocation = OwnerActor->GetActorLocation();
	}
	else
	{
		const FVector SocketLocation = ChildActorComponent->GetSocketLocation(SocketName);
		Direction = LastSocketPosition - ChildActorComponent->GetSocketLocation(SocketName);
		Direction.Normalize();

		StartLocation = OwnerActor->GetActorLocation() + GetRelativeLocation();
	}

	FVector EndLocation = StartLocation * Direction * 1000.0f;

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(TargetObjectType);

	UClass* SeekClass = nullptr;

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Init(OwnerActor, 1);

	TArray<FHitResult> OutHits;
	
	UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), StartLocation, EndLocation, CastSphereRadius,
		TraceObjectTypes, false, IgnoreActors, EDrawDebugTrace::ForDuration, OutHits, true,
		FLinearColor::Red, FLinearColor::Green, 3.0f);

	for (FHitResult Hit : OutHits)
	{
		if (Hit.Actor == Actor)
		{
			return true;
		}
	}

	return false;
}

void URGX_HitboxComponent::SetChildActorAndSocket(UChildActorComponent* NewChildActorComponent, const FName NewSocketName)
{
	ChildActorComponent = NewChildActorComponent;
	SocketName = NewSocketName;
}

bool URGX_HitboxComponent::HasChildActor()
{
	return ChildActorComponent != nullptr;
}

void URGX_HitboxComponent::ApplyEffects(AActor* OtherActor)
{
	//UE_LOG(LogTemp, Warning, TEXT("Apply putos efectos\n"));
	if (DefaultEffectToApply || AbilityEffectsInfo.GameplayEffectsToTarget.Num() > 0 || AbilityEffectsInfo.GameplayEventsToTarget.Num() > 0
		|| AbilityEffectsInfo.GameplayEffectsToOwner.Num() > 0 || AbilityEffectsInfo.GameplayEventsToOwner.Num() > 0)
	{
		USceneComponent* Parent = GetAttachParent();
		AActor* OwnerActor = Parent->GetAttachmentRootActor();

		if (!OwnerActor)
		{
			OwnerActor = GetOwner();
		}

		// Try to get owner ASC
		UAbilitySystemComponent* ApplierASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerActor);
		UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor);
		// If not fallback to target
		if (!ApplierASC)
		{
			ApplierASC = TargetASC;
		}

		// Only apply if ASC valid
		if (ApplierASC && TargetASC)
		{
			// Default Effect to apply
			if (DefaultEffectToApply)
			{
				ApplierASC->ApplyGameplayEffectToTarget(DefaultEffectToApply->GetDefaultObject<UGameplayEffect>(), TargetASC, 1, ApplierASC->MakeEffectContext());
			}
			
			// Default Events to apply
			for (FRGX_HitboxGameplayEvent& DefaultEvent : DefaultEventsToApply)
			{
				if (DefaultEvent.bActivated == true)
				{
					DefaultEvent.EventData.Instigator = OwnerActor;
					TargetASC->HandleGameplayEvent(DefaultEvent.GameplayEvent, &DefaultEvent.EventData);
				}
			}

			// Effects and Events to apply that come from an ability activation
			for (TSubclassOf<UGameplayEffect> Effect : AbilityEffectsInfo.GameplayEffectsToTarget) // TODO: BUGARDO
			{
				ApplierASC->ApplyGameplayEffectToTarget(Effect->GetDefaultObject<UGameplayEffect>(), TargetASC, 1, ApplierASC->MakeEffectContext());
			}

			for (TSubclassOf<UGameplayEffect> Effect : AbilityEffectsInfo.GameplayEffectsToOwner)
			{
				ApplierASC->ApplyGameplayEffectToSelf(Effect->GetDefaultObject<UGameplayEffect>(), 1, ApplierASC->MakeEffectContext());

			}

			for (int i = 0; i < AbilityEffectsInfo.GameplayEventsToTarget.Num(); ++i)
			{
				FGameplayTag EventTag = AbilityEffectsInfo.GameplayEventsToTarget[i]->EventTag;
				FGameplayEventData EventData = {};
				EventData.Instigator = OwnerActor;
				EventData.OptionalObject = AbilityEffectsInfo.GameplayEventsToTarget[i];
				TargetASC->HandleGameplayEvent(EventTag, &EventData);
			}

			for (int i = 0; i < AbilityEffectsInfo.GameplayEventsToOwner.Num(); ++i)
			{
				FGameplayTag EventTag = AbilityEffectsInfo.GameplayEventsToOwner[i]->EventTag;
				FGameplayEventData EventData = {};
				EventData.Instigator = OwnerActor;
				EventData.OptionalObject = AbilityEffectsInfo.GameplayEventsToOwner[i];
				ApplierASC->HandleGameplayEvent(EventTag, &EventData);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No effect or event to apply with the hitbox"));
	}
}

void URGX_HitboxComponent::OnComponentOverlap(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("On Component Overlap Hitbox\n"));
	if (bEffectActivated == false)
		return;

	for (AActor* Hit : ActorsHit)
	{
		// An actor cannot be hit more than once by the same hitbox activation
		if (OtherActor == Hit)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Hit Same Actor\n"));
			return;
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("Overlap\n"));
	USceneComponent* Parent = GetAttachParent();
	AActor* OwnerActor = Parent->GetAttachmentRootActor();

	if (!OwnerActor)
	{
		OwnerActor = GetOwner();
	}
	

	//UE_LOG(LogTemp, Warning, TEXT("Other component name is %s."), *OtherComp->GetName());
	URGX_HitboxComponent* HitboxComponent = Cast<URGX_HitboxComponent>(OtherComp->GetAttachParent());

	ETeamAttitude::Type Attitude = FGenericTeamId::GetAttitude(OwnerActor, OtherActor);
	const FGameplayTagContainer BlockingTags = TagsToBlockTheHit;
	IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(OtherActor);
	bool CanApplyEffect = false;
	if (TagInterface )
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hitbox Overlap"));

		// Check and may rethink this ... If BlockingTags is empty, this returns true
		if (TagInterface->HasAllMatchingGameplayTags(BlockingTags) == false || BlockingTags.IsEmpty())
			CanApplyEffect = true;
	}

	if (Attitude == TeamToApply && HitboxComponent == nullptr && CanApplyEffect)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hitbox Overlap"));
		ARGX_PlayerCharacter* player = Cast<ARGX_PlayerCharacter>(OwnerActor);
		if (player)
		{
			//OwnerActor->CustomTimeDilation = 0.05f;
			//OtherActor->CustomTimeDilation = 0.05f;
			//AGameStateBase* GameState = GetWorld()->GetGameState();
			//FTimerDelegate PunchDelegate = FTimerDelegate::CreateUObject(this, &URGX_HitboxComponent::ResetCustomTimeDilation, OwnerActor, OtherActor);
			//GetWorld()->GetTimerManager().SetTimer(PunchTimerHandle, PunchDelegate, 0.15, false);
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.05f);
			GetWorld()->GetTimerManager().SetTimer(PunchTimerHandle, this, &URGX_HitboxComponent::ResetCustomTimeDilation, 0.06666 * 0.05f, false);
		}

		ActorsHit.Add(OtherActor);
		ApplyEffects(OtherActor);
	}

	switch (DestroyOnOverlap)
	{
	case ERGX_DestroyOnOverlapType::Overlap:
		DestroyOwnerOnOverlap();
		break;
	case ERGX_DestroyOnOverlapType::Hostile:
		if (Attitude == ETeamAttitude::Type::Hostile)
			DestroyOwnerOnOverlap();
		break;
	case ERGX_DestroyOnOverlapType::Dynamic:
		if (OtherActor->IsRootComponentMovable())
			DestroyOwnerOnOverlap();
		break;
	case ERGX_DestroyOnOverlapType::EffectApplied:
		if (CanApplyEffect && Attitude == ETeamAttitude::Type::Hostile)
			DestroyOwnerOnOverlap();
		break;
	default:
		break;
	}
}

void URGX_HitboxComponent::DestroyOwnerOnOverlap()
{
	GetOwner()->Destroy();
}

bool URGX_HitboxComponent::CheckIfEffectIsApplied(AActor* TargetActor)
{
	// If TargetActor met the requirements to avoid the effect, return false.
	IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(TargetActor);
	if (TagInterface)
	{
		const FGameplayTagContainer& BlockingTags = TagsToBlockTheHit;
		if (TagInterface->HasAllMatchingGameplayTags(BlockingTags))
			return false;
	}
	return true;
}

void URGX_HitboxComponent::ResetCustomTimeDilation()
{
	//Owner->CustomTimeDilation = 1.0f;
	//Other->CustomTimeDilation = 1.0f;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}
