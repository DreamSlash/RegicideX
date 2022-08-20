// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_Peasant_Shield.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

ARGX_Peasant_Shield::ARGX_Peasant_Shield() 
{

}

void ARGX_Peasant_Shield::HandleDamage(float DamageAmount, const FHitResult& HitInfo, const FGameplayTagContainer& DamageTags, ARGX_CharacterBase* InstigatorCharacter, AActor* DamageCauser)
{
	const FVector StartPos = GetActorLocation() + GetActorForwardVector() * 200.0f;
	const FVector EndPos = StartPos + GetActorForwardVector() * 2000.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
	TArray<AActor*> ActorsToIgnore;
	FHitResult OutHit;
	bool Hitted = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), StartPos, EndPos, 100.0f, ObjectTypesArray, false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true);

	if (ShieldAmount > 0.0f && Hitted && OutHit.Actor == TargetActor)
	{

		ShieldAmount -= 50.0f;
		AAIController* AICont = Cast<AAIController>(this->GetController());
		UBlackboardComponent* BB = AICont->GetBlackboardComponent(); 
		BB->SetValueAsBool("bWasHit", true);
		PlayAnimMontage(AMShieldBlock); 

	}
	else 
	{
		Super::HandleDamage(DamageAmount, HitInfo, DamageTags, InstigatorCharacter, DamageCauser);
	}
}
