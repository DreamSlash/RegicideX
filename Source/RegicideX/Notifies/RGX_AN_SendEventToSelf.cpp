
#include "RegicideX/Notifies/RGX_AN_SendEventToSelf.h"
#include "AbilitySystemComponent.h"
#include "RegicideX/Actors/RGX_CharacterBase.h"
#include "RegicideX/GAS/RGX_PayloadObjects.h"

void URGX_AN_SendEventToSelf::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* Owner = MeshComp->GetOwner();
	if (Owner)
	{
		ARGX_CharacterBase* OwnerCharacter = Cast<ARGX_CharacterBase>(Owner);

		UAbilitySystemComponent* ACS = OwnerCharacter ? OwnerCharacter->GetAbilitySystemComponent() : nullptr;
		if (ACS)
		{
			FGameplayEventData EventData;
			EventData.EventTag = EventTag;
			EventData.Instigator = Owner;
			EventData.OptionalObject = EventPayload;
			ACS->HandleGameplayEvent(EventTag, &EventData);
		}
	}
}
