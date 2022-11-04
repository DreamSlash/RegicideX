// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_Peasant_Shield.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RegicideX/Character/RGX_PlayerCharacter.h"

ARGX_Peasant_Shield::ARGX_Peasant_Shield() 
{

}

bool ARGX_Peasant_Shield::CanBeLaunched(AActor* ActorInstigator, URGX_LaunchEventDataAsset* LaunchPayload)
{
	bool bResult = Super::CanBeLaunched(ActorInstigator, LaunchPayload);
	if (bResult == false) return false;

	// if shield is not available the peasant can be launched
	if (ShieldAmount <= 0.0f == true) return true;

	// if shield is available and the launch attack comes from the front, the launch is mitigated
	FVector MyForward = GetActorForwardVector();
	MyForward.Z = 0.0f;
	MyForward.Normalize();

	FVector ToTarget = ActorInstigator->GetActorLocation() - GetActorLocation();
	ToTarget.Z = 0.0f;
	ToTarget.Normalize();

	const float DotProduct = FVector::DotProduct(MyForward, ToTarget);

	UE_LOG(LogTemp, Warning, TEXT("Dot Product: %f"), DotProduct);

	if (DotProduct > 0.3f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Shield canceled launch"));
		return false;
	}

	return true;
}

float ARGX_Peasant_Shield::HandleDamageMitigation(float DamageAmount, const FHitResult& HitInfo, const FGameplayTagContainer& DamageTags, ARGX_CharacterBase* InstigatorCharacter, AActor* DamageCauser)
{
	if (bImmune)
		return 0.0f;

	if (TargetActor == nullptr) return 0.0f;

	if (ShieldAmount > 0.0f == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Shield did not mitigate damage"));
		return DamageAmount;
	}

	ARGX_PlayerCharacter* Player = Cast<ARGX_PlayerCharacter>(InstigatorCharacter);

	FVector MyForward = GetActorForwardVector();
	MyForward.Z = 0.0f;
	MyForward.Normalize();

	FVector ToTarget = TargetActor->GetActorLocation() - GetActorLocation();
	ToTarget.Z = 0.0f;
	ToTarget.Normalize();

	const float DotProduct = FVector::DotProduct(MyForward, ToTarget);

	UE_LOG(LogTemp, Warning, TEXT("Dot Product: %f"), DotProduct);

	if (DotProduct > 0.5f)
	{
		// If attack is a HeavyAttack, shield takes damage.
		if (Player && Player->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Ability.Player.HeavyAttack"))))
		{
			TArray<UStaticMeshComponent*> Components;
			GetComponents<UStaticMeshComponent>(Components);
			UStaticMeshComponent* ShieldMesh = nullptr;
			for (UStaticMeshComponent* Component : Components)
			{
				if (Component->GetName() == FString("Shield"))
				{
					ShieldMesh = Component;
					OnShieldCracked();
				}
			}
			UE_LOG(LogTemp, Warning, TEXT("Shield is damaged! Attack was heavy."));
			ShieldAmount -= DamageAmount;
			if (ShieldAmount > 0.0f) 
			{
				PlayAnimMontage(AMShieldBlockBreaks);
			}
			else {
				PlayAnimMontage(AMShieldBreaks);
				if (ShieldMesh) 
					ShieldMesh->DestroyComponent();
			}
			return 0.0f;
		}

		PlayAnimMontage(AMShieldBlock);
		UE_LOG(LogTemp, Warning, TEXT("Shield mitigated damage. Shield health: %d"), ShieldAmount);
		AAIController* AICont = Cast<AAIController>(this->GetController());
		UBlackboardComponent* BB = AICont->GetBlackboardComponent();
		BB->SetValueAsBool("bWasHit", true);
		return 0.0f;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Shield did not mitigate damage"));
		return DamageAmount;
	}
}
