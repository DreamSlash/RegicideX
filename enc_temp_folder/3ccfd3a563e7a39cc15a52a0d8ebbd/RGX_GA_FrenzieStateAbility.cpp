// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/GAS/GameplayAbilities/Peasant/Suicidal/RGX_GA_FrenzieStateAbility.h"
#include "RegicideX/Actors/Enemies/RGX_Peasant.h"
#include "GameFramework/CharacterMovementComponent.h"

URGX_GA_FrenzieStateAbility::URGX_GA_FrenzieStateAbility()
{

}

bool URGX_GA_FrenzieStateAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
    if (bResult == false)
        return false;

    ARGX_Peasant* Peasant = Cast<ARGX_Peasant>(GetAvatarActorFromActorInfo());
    if (Peasant == nullptr)
        return false;

    return true;
}

void URGX_GA_FrenzieStateAbility::OnReceivedEvent(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag.MatchesTagExact(FrenzieTag))
	{
        ARGX_Peasant* Peasant = Cast<ARGX_Peasant>(GetAvatarActorFromActorInfo());
        if (Peasant)
        {
            UCharacterMovementComponent* CharacterMovementComponent = Peasant->GetCharacterMovement();
            if (CharacterMovementComponent)
            {
                CharacterMovementComponent->MaxWalkSpeed *= FrenzieSpeedMultiplier;
            }
        }
	}
}