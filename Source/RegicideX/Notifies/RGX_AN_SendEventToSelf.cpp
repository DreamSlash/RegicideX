// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/Notifies/RGX_AN_SendEventToSelf.h"
#include "AbilitySystemComponent.h"
#include "RegicideX/GAS/RGX_PayloadObjects.h"

void URGX_AN_SendEventToSelf::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* Owner = MeshComp->GetOwner();

	UAbilitySystemComponent* ACS = Owner->FindComponentByClass<UAbilitySystemComponent>();
	if (ACS)
	{
		FGameplayEventData EventData;
		EventData.EventTag = EventTag;
		EventData.Instigator = Owner;
		EventData.OptionalObject = EventPayload;
		ACS->HandleGameplayEvent(EventTag, &EventData);
	}
}
