// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/Notifies/RGX_AN_FallAttackHeightCheck.h"
#include "RegicideX/Character/RGX_PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemComponent.h"

void URGX_AN_FallAttackHeightCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	const AActor* Owner = MeshComp->GetOwner();

	const ARGX_PlayerCharacter* PlayerCharacter = Cast<ARGX_PlayerCharacter>(Owner);
	if(PlayerCharacter)
	{
		const FVector TraceStart = PlayerCharacter->GetActorLocation();
		const FVector TraceEnd = TraceStart - FVector(0.0f, 0.0f, 400.0f);

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

		const TArray<AActor*> ActorsToIgnore;

		FHitResult OutHit;

		UWorld* World = Owner->GetWorld();

		bool bResult = UKismetSystemLibrary::LineTraceSingleForObjects(
			World, TraceStart, TraceEnd, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit, true, FLinearColor::Red, FLinearColor::Green, 3.0f);
	
		if (bResult == true)
		{
			UAbilitySystemComponent* ACS = Owner->FindComponentByClass<UAbilitySystemComponent>();
			if (ACS)
			{
				FGameplayEventData EventData;
				ACS->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("GameplayEvent.GoingToLand")), &EventData);
			}
		}
	}
}
