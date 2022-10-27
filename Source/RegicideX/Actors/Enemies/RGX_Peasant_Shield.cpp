// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_Peasant_Shield.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

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

	if (ShieldAmount > 0.0f == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Shield did not mitigate damage"));
		return DamageAmount;
	}

	/*
	const FVector StartPos = GetActorLocation() + GetActorForwardVector() * 200.0f;
	const FVector EndPos = StartPos + GetActorForwardVector() * 2000.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
	TArray<AActor*> ActorsToIgnore;
	FHitResult OutHit;
	bool Hitted = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), StartPos, EndPos, 100.0f, ObjectTypesArray, false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true);
	*/

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
		UE_LOG(LogTemp, Warning, TEXT("Shield mitigated damage"));
		ShieldAmount -= 50.0f;
		AAIController* AICont = Cast<AAIController>(this->GetController());
		UBlackboardComponent* BB = AICont->GetBlackboardComponent();
		BB->SetValueAsBool("bWasHit", true);
		PlayAnimMontage(AMShieldBlock);
		return 0.0f;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Shield did not mitigate damage"));
		return DamageAmount;
	}
}
