
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

DEFINE_LOG_CATEGORY(LogHitbox);

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
			Shapes.Add(ShapeComponent);
		}
	}

	// Delegate Shape OnComponentBeginOverlap to custom OnCompnentOverlap
	for (UShapeComponent* shape : Shapes)
	{
		shape->OnComponentBeginOverlap.AddDynamic(this, &URGX_HitboxComponent::OnComponentOverlap);
	}
	
	bStartActive ? ActivateHitbox() : DeactivateHitbox();
}

void URGX_HitboxComponent::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	for (UShapeComponent* shape : Shapes)
	{
		shape->OnComponentBeginOverlap.RemoveDynamic(this, &URGX_HitboxComponent::OnComponentOverlap);
	}
}

void URGX_HitboxComponent::OnComponentEndOverlap(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("OnComponentEndOverlap: %s - %s"), *GetOwner()->GetName(), *OtherActor->GetName());
}

void URGX_HitboxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (bIsStatic)
		return;

	// Tick component to get last position of socket for being able to calculate velocity direction later
	const ECollisionEnabled::Type CollisionType = GetCollisionEnabled();
	if (CollisionType == ECollisionEnabled::NoCollision)
		return;
}

void URGX_HitboxComponent::ActivateHitbox()
{
	const USceneComponent* Parent = GetAttachParent();
	AActor* OwnerActor = Parent->GetAttachmentRootActor();

	if (OwnerActor == nullptr)
	{
		OwnerActor = GetOwner();
	}

	for (UShapeComponent* Shape : Shapes)
	{
		Shape->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Shape->SetCollisionProfileName("Dodgeable");
	}
}

void URGX_HitboxComponent::DeactivateHitbox()
{
	for (UShapeComponent* Shape : Shapes)
	{
		Shape->SetCollisionProfileName("Dodgeable");
		Shape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	ActorsHit.Empty();
}

void URGX_HitboxComponent::SetEventTag(const FGameplayTag& NewTag)
{
	EventTag = NewTag;
}

//void URGX_HitboxComponent::SetAbilityEffectsInfo(const FRGX_AbilityEffectsInfo& NewAbilityEffectsInfo)
//{
//	AbilityEffectsInfo = NewAbilityEffectsInfo;
//}
//
//void URGX_HitboxComponent::RemoveAbilityEffectsInfo()
//{
//	AbilityEffectsInfo.EffectContextHandle = FGameplayEffectContextHandle();
//	AbilityEffectsInfo.GameplayEffectsToTarget.Empty();
//	AbilityEffectsInfo.GameplayEventsToTarget.Empty();
//	AbilityEffectsInfo.GameplayEffectsToOwner.Empty();
//	AbilityEffectsInfo.GameplayEventsToOwner.Empty();
//}

//void URGX_HitboxComponent::SetGameplayEffectContextHandle(FGameplayEffectContextHandle Handle)
//{
//	DefaultGameplayEffectContextHandle = Handle;
//}

// TODO [REFACTOR]: This functions should take into account the multiple shapes this class can have
bool URGX_HitboxComponent::IsGoingToOverlapActor(AActor* Actor)
{
	// if it is static we do not check if will overlap in the future, because it does not move
	if (bIsStatic)
		return false;

	const USceneComponent* Parent = GetAttachParent();
	AActor* OwnerActor = Parent->GetAttachmentRootActor();

	const FVector ActorVelocity = Actor->GetVelocity();
	const FVector HitboxVelocity = OwnerActor->GetVelocity();
	const FVector VelocityVector = HitboxVelocity - ActorVelocity;

	const FVector StartLocation = OwnerActor->GetActorLocation();
	const FVector EndLocation = StartLocation + 0.5f * VelocityVector; // where it will be the collision in 0.5s respect to the Actor being tested

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

//void URGX_HitboxComponent::ApplyEffects(AActor* OtherActor)
//{
//	//UE_LOG(LogTemp, Warning, TEXT("Apply putos efectos\n"));
//	if (DefaultEffectToApply || AbilityEffectsInfo.GameplayEffectsToTarget.Num() > 0 || AbilityEffectsInfo.GameplayEventsToTarget.Num() > 0
//		|| AbilityEffectsInfo.GameplayEffectsToOwner.Num() > 0 || AbilityEffectsInfo.GameplayEventsToOwner.Num() > 0)
//	{
//		USceneComponent* Parent = GetAttachParent();
//		AActor* OwnerActor = Parent->GetAttachmentRootActor();
//
//		if (!OwnerActor)
//		{
//			OwnerActor = GetOwner();
//		}
//
//		// Try to get owner ASC
//		UAbilitySystemComponent* ApplierASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerActor);
//		UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor);
//		// If not fallback to target
//		if (!ApplierASC)
//		{
//			ApplierASC = TargetASC;
//		}
//
//		// Only apply if ASC valid
//		if (ApplierASC && TargetASC)
//		{
//			// Default Effect to apply
//			if (DefaultEffectToApply)
//			{
//				FGameplayEffectContextHandle ContextHandle;
//				if (DefaultGameplayEffectContextHandle.Get())
//				{
//					ContextHandle = DefaultGameplayEffectContextHandle;
//				}
//				else
//				{
//					ContextHandle = ApplierASC->MakeEffectContext();
//				}
//
//				ApplierASC->ApplyGameplayEffectToTarget(DefaultEffectToApply->GetDefaultObject<UGameplayEffect>(), TargetASC, 1, ContextHandle);
//			}
//			
//			// Default Events to apply
//			for (FRGX_HitboxGameplayEvent& DefaultEvent : DefaultEventsToApply)
//			{
//				if (DefaultEvent.bActivated == true)
//				{
//					DefaultEvent.EventData.Instigator = OwnerActor;
//					TargetASC->HandleGameplayEvent(DefaultEvent.GameplayEvent, &DefaultEvent.EventData);
//				}
//			}
//
//			// Effects and Events to apply that come from an ability activation
//			float AbilityLevel = AbilityEffectsInfo.EffectContextHandle.GetAbilityLevel();
//
//			FGameplayEffectContextHandle ContextHandle = AbilityEffectsInfo.EffectContextHandle.Get() ? AbilityEffectsInfo.EffectContextHandle : ApplierASC->MakeEffectContext();
//
//			for (TSubclassOf<UGameplayEffect> Effect : AbilityEffectsInfo.GameplayEffectsToTarget) // TODO: BUGARDO
//			{
//				ApplierASC->ApplyGameplayEffectToTarget(Effect->GetDefaultObject<UGameplayEffect>(), TargetASC, AbilityLevel, ContextHandle);
//			}
//
//			for (TSubclassOf<UGameplayEffect> Effect : AbilityEffectsInfo.GameplayEffectsToOwner)
//			{
//				ApplierASC->ApplyGameplayEffectToSelf(Effect->GetDefaultObject<UGameplayEffect>(), AbilityLevel, ContextHandle);
//
//			}
//
//			for (int i = 0; i < AbilityEffectsInfo.GameplayEventsToTarget.Num(); ++i)
//			{
//				FGameplayTag EventTag = AbilityEffectsInfo.GameplayEventsToTarget[i]->EventTag;
//				FGameplayEventData EventData = {};
//				EventData.Instigator = OwnerActor;
//				EventData.OptionalObject = AbilityEffectsInfo.GameplayEventsToTarget[i];
//				TargetASC->HandleGameplayEvent(EventTag, &EventData);
//			}
//
//			for (int i = 0; i < AbilityEffectsInfo.GameplayEventsToOwner.Num(); ++i)
//			{
//				FGameplayTag EventTag = AbilityEffectsInfo.GameplayEventsToOwner[i]->EventTag;
//				FGameplayEventData EventData = {};
//				EventData.Instigator = OwnerActor;
//				EventData.OptionalObject = AbilityEffectsInfo.GameplayEventsToOwner[i];
//				ApplierASC->HandleGameplayEvent(EventTag, &EventData);
//			}
//		}
//	}
//	else
//	{
//		UE_LOG(LogTemp, Error, TEXT("No effect or event to apply with the hitbox"));
//	}
//}

void URGX_HitboxComponent::OnComponentOverlap(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{	
	// Check if the actor being hit has previously been hit. 
	// It cannot be hit more than once by the same hitbox activation.
	for (AActor* Hit : ActorsHit)
	{
		if (OtherActor == Hit)
			return;
	}

	const USceneComponent* Parent	= GetAttachParent();
	AActor* OwnerActor				= Parent->GetAttachmentRootActor();
	if (!OwnerActor)
	{
		OwnerActor = GetOwner();
	}

	UE_LOG(LogHitbox, Display, TEXT("OnComponentOverlap: %s - %s"), *OwnerActor->GetName(), *OtherActor->GetName());

	const FGameplayTagContainer BlockingTags = TagsToBlockTheHit;
	IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(OtherActor);
	bool CanApplyEffect = false;
	if (TagInterface )
	{
		if (TagInterface->HasAllMatchingGameplayTags(BlockingTags) == false || BlockingTags.IsEmpty())
			CanApplyEffect = true;
	}

	ETeamAttitude::Type Attitude = FGenericTeamId::GetAttitude(OwnerActor, OtherActor);
	if (Attitude == TeamToApply && CanApplyEffect)
	{
		const ARGX_CharacterBase* OwnerCharacter = Cast<ARGX_CharacterBase>(OwnerActor);
		UAbilitySystemComponent* OwnerAbilitySystemComponent = OwnerCharacter->GetAbilitySystemComponent();

		FGameplayEventData *Payload = new FGameplayEventData();
		Payload->Instigator = OwnerActor;
		Payload->Target		= OtherActor;

		for(const FGameplayTag tag : EffectTags)
			OwnerAbilitySystemComponent->HandleGameplayEvent(tag, Payload);

		ActorsHit.Add(OtherActor);
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

void URGX_HitboxComponent::ResetCustomTimeDilation()
{
	if (Owner == nullptr && ActorsWithTimeDilation.Num() == 0)
		return;
	UE_LOG(LogHitbox, Display, TEXT("ResetCustomTimeDilation"));
	Owner->CustomTimeDilation = 1.0f;
	Owner = nullptr;
	for (const auto& Enemy : ActorsWithTimeDilation) {
		Enemy->CustomTimeDilation = 1.0f;
	}
	ActorsWithTimeDilation.Empty();
}
