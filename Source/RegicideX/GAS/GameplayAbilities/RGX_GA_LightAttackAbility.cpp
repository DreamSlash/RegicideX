#include "RGX_GA_LightAttackAbility.h"
#include "GameFramework/Character.h"

URGX_LightAttackAbility::URGX_LightAttackAbility()
{
	//static ConstructorHelpers::FObjectFinder<UAnimMontage> AnimMontage(TEXT("/Game/RXGame/Player/Animations/Montages/AM_LightAttack"));

	//MontageToPlay = AnimMontage.Object;
}

void URGX_LightAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	ACharacter* character = Cast<ACharacter>(ActorInfo->AvatarActor);
	if (character)
	{
		character->PlayAnimMontage(MontageToPlay, 1.0f, StartSectionName);
	}

	UE_LOG(LogTemp, Warning, TEXT("Light Attack Ability"));
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);

}
