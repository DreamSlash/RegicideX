#include "RGX_HitboxComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "Components/ChildActorComponent.h"
#include "GenericTeamAgentInterface.h"
#include "../GAS/RGX_PayloadObjects.h"

URGX_HitboxComponent::URGX_HitboxComponent()
{

}

void URGX_HitboxComponent::BeginPlay()
{
	Super::BeginPlay();
	OnComponentBeginOverlap.AddDynamic(this, &URGX_HitboxComponent::OnComponentOverlap);

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

void URGX_HitboxComponent::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	OnComponentBeginOverlap.RemoveDynamic(this, &URGX_HitboxComponent::OnComponentOverlap);
}

void URGX_HitboxComponent::ActivateHitbox()
{
	USceneComponent* Parent = GetAttachParent();
	AActor* OwnerActor = Parent->GetAttachmentRootActor();

	if (!OwnerActor)
	{
		OwnerActor = GetOwner();
	}

	//UE_LOG(LogTemp, Warning, TEXT("Activate Hitbox\n"));
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionProfileName("Dodgeable");
}

void URGX_HitboxComponent::DeactivateHitbox()
{
	//UE_LOG(LogTemp, Warning, TEXT("Deactivate Hitbox\n"));
	SetCollisionProfileName("Dodgeable");
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void URGX_HitboxComponent::ActivateEffect()
{
	//UE_LOG(LogTemp, Warning, TEXT("Activate Effect\n"));
	bEffectActivated = true;
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionProfileName("Dodgeable");
}

void URGX_HitboxComponent::DeactivateEffect()
{
	//UE_LOG(LogTemp, Warning, TEXT("Deactivate Effect\n"));
	bEffectActivated = false;
	SetCollisionProfileName("Dodgeable");
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void URGX_HitboxComponent::ActivateEvent(const FGameplayTag& EventTag)
{
	for (FRGX_HitboxGameplayEvent& Event : EventsToApply)
	{
		if (Event.GameplayEvent == EventTag)
		{
			Event.bActivated = true;
		}
	}
}

void URGX_HitboxComponent::DeactivateEvent(const FGameplayTag& EventTag)
{
	for (FRGX_HitboxGameplayEvent& Event : EventsToApply)
	{
		if (Event.GameplayEvent == EventTag)
		{
			Event.bActivated = false;
		}
	}
}

void URGX_HitboxComponent::AddEvent(const FGameplayTag& EventTag, const FGameplayEventData& EventData, bool bAutoActivateEvent)
{
	FRGX_HitboxGameplayEvent NewEvent = {};
	NewEvent.GameplayEvent = EventTag;
	NewEvent.EventData = EventData;
	NewEvent.bActivated = false;

	EventsToApply.Add(NewEvent);
}

void URGX_HitboxComponent::RemoveEvent(const FGameplayTag& EventTag)
{
	for (int i = 0; i < EventsToApply.Num(); ++i)
	{
		if (EventsToApply[i].GameplayEvent == EventTag)
		{
			EventsToApply.RemoveAt(i); // TODO: Faig removeAt enlloc de remove perque "==" no esta definit i em fa pal definir-lo
			break;
		}
	}
}

void URGX_HitboxComponent::SetAbilityEffectsInfo(FRGX_AbilityEffectsInfo& NewAbilityEffectsInfo)
{
	AbilityEffectsInfo = NewAbilityEffectsInfo;
}

void URGX_HitboxComponent::RemoveAbilityEffectsInfo()
{
	AbilityEffectsInfo.GameplayEffectsToTarget.Empty();
	AbilityEffectsInfo.GameplayEventsToTarget.Empty();
	AbilityEffectsInfo.GameplayEffectsToOwner.Empty();
	AbilityEffectsInfo.GameplayEventsToOwner.Empty();
}

void URGX_HitboxComponent::ApplyEffects(AActor* OtherActor)
{
	if (EffectToApply || AbilityEffectsInfo.GameplayEffectsToTarget.Num() > 0 || AbilityEffectsInfo.GameplayEventsToTarget.Num() > 0
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
			if (EffectToApply)
			{
				ApplierASC->ApplyGameplayEffectToTarget(EffectToApply->GetDefaultObject<UGameplayEffect>(), TargetASC, 1, ApplierASC->MakeEffectContext());
			}
			
			// Default Events to apply
			for (FRGX_HitboxGameplayEvent& DefaultEvent : EventsToApply)
			{
				if (DefaultEvent.bActivated == true)
				{
					DefaultEvent.EventData.Instigator = OwnerActor;
					TargetASC->HandleGameplayEvent(DefaultEvent.GameplayEvent, &DefaultEvent.EventData);
				}
			}

			// Effects and Events to apply that come from an ability activation
			for (TSubclassOf<UGameplayEffect> Effect : AbilityEffectsInfo.GameplayEffectsToTarget)
			{
				ApplierASC->ApplyGameplayEffectToTarget(Effect->GetDefaultObject<UGameplayEffect>(), TargetASC, 1, ApplierASC->MakeEffectContext());
			}

			for (FRGX_AbilityGameplayEvent AbilityEvent : AbilityEffectsInfo.GameplayEventsToTarget)
			{
				AbilityEvent.EventData.Instigator = OwnerActor;
				TargetASC->HandleGameplayEvent(AbilityEvent.GameplayEvent, &AbilityEvent.EventData);
			}

			for (TSubclassOf<UGameplayEffect> Effect : AbilityEffectsInfo.GameplayEffectsToOwner)
			{
				ApplierASC->ApplyGameplayEffectToSelf(Effect->GetDefaultObject<UGameplayEffect>(), 1, ApplierASC->MakeEffectContext());

			}

			for (FRGX_AbilityGameplayEvent AbilityEvent : AbilityEffectsInfo.GameplayEventsToOwner)
			{
				AbilityEvent.EventData.Instigator = OwnerActor;
				ApplierASC->HandleGameplayEvent(AbilityEvent.GameplayEvent, &AbilityEvent.EventData);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No effect or event to apply with the hitbox"));
	}
}

void URGX_HitboxComponent::OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("On Component Overlap Hitbox\n"));
	if (bEffectActivated == false)
		return;

	//UE_LOG(LogTemp, Warning, TEXT("Overlap\n"));
	USceneComponent* Parent = GetAttachParent();
	AActor* OwnerActor = Parent->GetAttachmentRootActor();

	if (!OwnerActor)
	{
		OwnerActor = GetOwner();
	}

	/*
	if (OwnerActor)
	{
		FString ActorName = OwnerActor->GetName();
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ActorName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO ATTACHED PARENT\n"));
	}
	*/

	const IGenericTeamAgentInterface* TeamAgentA = Cast<const IGenericTeamAgentInterface>(OwnerActor);

	/*
	if (TeamAgentA)
	{
		uint8 teamID = TeamAgentA->GetGenericTeamId().GetId();
		UE_LOG(LogTemp, Warning, TEXT("TEAM ID: %d\n"), teamID);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TeamAgentA is nullptr\n"));
	}
	*/
	
	ETeamAttitude::Type Attitude = FGenericTeamId::GetAttitude(OwnerActor, OtherActor);
	if (Attitude == TeamToApply)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hitbox Overlap"));
		ApplyEffects(OtherActor);
	}
}