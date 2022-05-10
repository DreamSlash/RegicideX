#include "RGX_MovementAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/CharacterMovementComponent.h"

void URGX_MovementAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMovementSpeedAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, GetMinMovementSpeed(), GetMaxMovementSpeed());

		// TODO: I think this shouldn't be here
		UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
		const AActor* Target = ASC->GetAvatarActor();
		UCharacterMovementComponent* CharacterMovementComponent = Cast<UCharacterMovementComponent>(Target->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
		CharacterMovementComponent->MaxWalkSpeed = NewValue;
	}
}

void URGX_MovementAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if (Attribute == GetMovementSpeedAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, GetMinMovementSpeed(), GetMaxMovementSpeed());
	}
}

void URGX_MovementAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
	const AActor* Target = ASC->GetAvatarActor();
	UCharacterMovementComponent* CharacterMovementComponent = Cast<UCharacterMovementComponent>(Target->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
	CharacterMovementComponent->MaxWalkSpeed = GetMovementSpeed();
}