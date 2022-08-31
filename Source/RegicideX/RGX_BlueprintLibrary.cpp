// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_BlueprintLibrary.h"
#include "RegicideX/GAS/RGX_GameplayEffectContext.h"
#include "PhysicsEngine/PhysicsSettings.h"

void URGX_BlueprintLibrary::ApplyGameplayEffectContextContainerToActor(const ARGX_CharacterBase* CharacterBase, TSubclassOf<UGameplayEffect> EffectToApply, URGX_RGXEventDataAsset* Payload)
{
	if (EffectToApply.Get() == nullptr) return;

	UAbilitySystemComponent* ACS = CharacterBase->GetAbilitySystemComponent();
	if (ACS)
	{
		FGameplayEffectContextHandle EffectContextHandle = ACS->MakeEffectContext();
		FGameplayEffectSpecHandle GameplayEffectSpecHandle = ACS->MakeOutgoingSpec(EffectToApply, CharacterBase->GetCharacterLevel(), EffectContextHandle);

		FRGX_GameplayEffectContext* RGXContextHandle = static_cast<FRGX_GameplayEffectContext*>(EffectContextHandle.Get());
		RGXContextHandle->OptionalObject = Payload;

		FGameplayEffectSpec* GESpec = GameplayEffectSpecHandle.Data.Get();
		ACS->ApplyGameplayEffectSpecToSelf(*GESpec);
	}
}

void URGX_BlueprintLibrary::LaunchCharacterToLocation(ARGX_CharacterBase* CharacterBase, const FVector& TargetLocation)
{
	UE_LOG(LogTemp, Warning, TEXT("Launch Character To Location. Default Gravity: %f"), UPhysicsSettings::Get()->DefaultGravityZ);

	if (CharacterBase == nullptr) return;

	const FVector CurrentLocation = CharacterBase->GetActorLocation();

	const float HorizontalDistance = FVector::Distance(FVector(CurrentLocation.X, CurrentLocation.Y, 0.0f), FVector(TargetLocation.X, TargetLocation.Y, 0.0f));
	const float Gravity = 3.0f * UPhysicsSettings::Get()->DefaultGravityZ;
	const float LaunchAngle = 45.0f;
	const float Sin = FMath::Sin(LaunchAngle * 2.0f);

	const float Velocity = FMath::Sqrt((HorizontalDistance * FMath::Abs(Gravity)) / Sin);
	
	const float xVelocity = Velocity * FMath::Sin(LaunchAngle);
	const float zVelocity = Velocity * FMath::Cos(LaunchAngle);

	FVector Direction = TargetLocation - CurrentLocation;
	Direction.Z = 0.0f;
	Direction.Normalize();
	const FVector HorizontalLaunchForce = Direction * xVelocity;

	const FVector LaunchVelocity = FVector(HorizontalLaunchForce.X, HorizontalLaunchForce.Y, zVelocity);

	CharacterBase->LaunchCharacter(LaunchVelocity, true, true);
}
