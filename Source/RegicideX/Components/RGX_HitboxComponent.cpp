
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
	
	bStartActive ? ActivateHitbox(true) : DeactivateHitbox();
}

void URGX_HitboxComponent::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	for (UShapeComponent* shape : Shapes)
	{
		shape->OnComponentBeginOverlap.RemoveDynamic(this, &URGX_HitboxComponent::OnComponentOverlap);
	}
}

void URGX_HitboxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bEffectActivated == false || bContinuousCollision == false) return;

	UWorld* World = GetWorld();
	if (World)
	{
		const float CurrentTime = World->GetTimeSeconds();
		if (CurrentTime - TimeSinceLastCollision > ContinuousCollisionRate)
		{
			TimeSinceLastCollision = CurrentTime;
			TSet<AActor*> OverlappingActors;
			GetOverlappingActors(OverlappingActors, ARGX_CharacterBase::StaticClass());
			for (AActor* OverlappedActor : OverlappingActors)
			{
				// TODO: Too much work to do a function who recovers all this info from all shapes in this hitboxcomponent
				HandleOverlappedActor(OverlappedActor, nullptr, 0, false, FHitResult());
			}
		}
	}
	/*
	if (bIsStatic)
		return;

	// Tick component to get last position of socket for being able to calculate velocity direction later
	const ECollisionEnabled::Type CollisionType = GetCollisionEnabled();
	if (CollisionType == ECollisionEnabled::NoCollision)
		return;
		*/
}

void URGX_HitboxComponent::GetOverlappingActors(TSet<AActor*>& OverlappingActors, TSubclassOf<AActor> ClassFilter) const
{
	for (UShapeComponent* shape : Shapes)
	{
		shape->GetOverlappingActors(OverlappingActors, ClassFilter);
	}

	//UE_LOG(LogTemp, Warning, TEXT("Num of overlapped actors: %d"), OverlappingActors.Num());
}

void URGX_HitboxComponent::ActivateHitbox(bool bActivateEffect)
{
	// Changed before enabling collisions
	bEffectActivated = bActivateEffect;

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
	//UE_LOG(LogTemp, Warning, TEXT("Deactivate Hitbox: %s"), *GetName());

	bEffectActivated = false;

	for (UShapeComponent* Shape : Shapes)
	{
		Shape->SetCollisionProfileName("Dodgeable");
		Shape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	ActorsHit.Empty();
}

void URGX_HitboxComponent::AddEventTag(const FGameplayTag& NewTag)
{
	EffectTags.Add(NewTag);
}

void URGX_HitboxComponent::RemoveEventTag(const FGameplayTag& Tag)
{
	EffectTags.Remove(Tag);
}

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

void URGX_HitboxComponent::OnComponentOverlap(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{	
	if (bEffectActivated == false)
		return;

	if (bContinuousCollision == true)
		return;

	// Check if the actor being hit has previously been hit. 
	// It cannot be hit more than once by the same hitbox activation.
	for (AActor* Hit : ActorsHit)
	{
		if (OtherActor == Hit)
			return;
	}

	HandleOverlappedActor(OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void URGX_HitboxComponent::HandleOverlappedActor(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const USceneComponent* Parent = GetAttachParent();
	AActor* OwnerActor = Parent->GetAttachmentRootActor();
	if (!OwnerActor)
	{
		OwnerActor = GetOwner();
	}

	bool bCanApplyEffects = CheckCanApplyEffect(OtherActor);
	ETeamAttitude::Type Attitude = FGenericTeamId::GetAttitude(OwnerActor, OtherActor);

	ARGX_CharacterBase* TargetCharacter = Cast<ARGX_CharacterBase>(OtherActor);
	if (TargetCharacter != nullptr)
	{
		FVector Direction = OwnerActor->GetActorLocation() - TargetCharacter->GetActorLocation();
		Direction.Normalize();
		TargetCharacter->HitReactDirection = Direction;
	}

	if (Attitude == TeamToApply && bCanApplyEffects)
	{
		SendCollisionEvents(OwnerActor, OtherActor, bFromSweep, SweepResult);

		ARGX_PlayerCharacter* Player = Cast<ARGX_PlayerCharacter>(OwnerActor);
		if(Player)
			OnHitting();
	}

	HandleDestroyOnOverlap(OtherActor, Attitude, bCanApplyEffects);
}

bool URGX_HitboxComponent::CheckCanApplyEffect(const AActor* OtherActor)
{
	const FGameplayTagContainer BlockingTags = TagsToBlockTheHit;
	const IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(OtherActor);
	if (TagInterface)
	{
		if (TagInterface->HasAllMatchingGameplayTags(BlockingTags) == false || BlockingTags.IsEmpty())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

void URGX_HitboxComponent::SendCollisionEvents(AActor* OwnerActor, AActor* OtherActor, bool bFromSweep, const FHitResult& SweepResult)
{
	const ARGX_CharacterBase* OwnerCharacter = Cast<ARGX_CharacterBase>(OwnerActor);
	if (OwnerCharacter)
	{
		UAbilitySystemComponent* OwnerAbilitySystemComponent = OwnerCharacter->GetAbilitySystemComponent();

		FGameplayEventData* Payload = new FGameplayEventData();
		Payload->Instigator = OwnerActor;
		Payload->Target = OtherActor;
		Payload->TargetTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Hitbox")));

		for (const FGameplayTag Tag : EffectTags)
			OwnerAbilitySystemComponent->HandleGameplayEvent(Tag, Payload);

		ActorsHit.Add(OtherActor);
	}

	if (OnHitboxOverlap.IsBound())
	{
		OnHitboxOverlap.Broadcast(OtherActor);
	}
}

void URGX_HitboxComponent::HandleDestroyOnOverlap(AActor* OtherActor, ETeamAttitude::Type Attitude, bool bCanApplyEffects)
{
	// TODO It probably should not be handled by hitbox component.
	// Check the case where hitbox should destroy the object.
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
		if (bCanApplyEffects && Attitude == ETeamAttitude::Type::Hostile)
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
