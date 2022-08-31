// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_Peasant_Shield.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

ARGX_Peasant_Shield::ARGX_Peasant_Shield() 
{

}

float ARGX_Peasant_Shield::HandleDamageMitigation(float DamageAmount, const FHitResult& HitInfo, const FGameplayTagContainer& DamageTags, ARGX_CharacterBase* InstigatorCharacter, AActor* DamageCauser)
{
	if (ShieldAmount > 0.0f == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Shield did not mitigate damage"));
		return DamageAmount;
	}

	const FVector StartPos = GetActorLocation() + GetActorForwardVector() * 200.0f;
	const FVector EndPos = StartPos + GetActorForwardVector() * 2000.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
	TArray<AActor*> ActorsToIgnore;
	FHitResult OutHit;
	bool Hitted = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), StartPos, EndPos, 100.0f, ObjectTypesArray, false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true);

	if (Hitted && OutHit.Actor == TargetActor)
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
