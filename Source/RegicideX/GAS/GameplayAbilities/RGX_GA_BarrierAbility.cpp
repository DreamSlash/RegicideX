#include "RGX_GA_BarrierAbility.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../../Actors/Enemies/RGX_EnemyBase.h"
#include "Components/MCV_AbilitySystemComponent.h"
#include "../RGX_PayloadObjects.h"
#include "UObject/UObjectGlobals.h"

/*
void URGX_BarrierAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Warning, TEXT("Barrier Ability"));
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}
*/

void URGX_BarrierAbility::CastBarrierSkill(AActor* CasterActor)
{
	FVector SpawnLocation = CasterActor->GetActorLocation();
	FRotator SpawnRotation = FRotator(0.0f);

	AActor* SpawnedBarrier = GetWorld()->SpawnActor<AActor>(BarrierClass, SpawnLocation, SpawnRotation);

	float radius = 200.0f;

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	UClass* SeekClass = ARGX_EnemyBase::StaticClass();

	TArray<AActor*> IgnoreActors;
	TArray<AActor*> OutActors;

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), SpawnLocation, radius, TraceObjectTypes, SeekClass, IgnoreActors, OutActors);

	for (AActor* Actor : OutActors)
	{
		ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(Actor);
		
		FVector EnemyLocation = Enemy->GetActorLocation();
		FVector DirectionVector = EnemyLocation - SpawnLocation;
		DirectionVector.Normalize();

		FVector ArrowVector = DirectionVector * 500.0f * FVector(0.0f, 0.0f, 200.0f);

		FVector LineStart = EnemyLocation;
		FVector LineEnd = EnemyLocation + ArrowVector;

		FLinearColor ArrowColor = FLinearColor::Red;

		UKismetSystemLibrary::DrawDebugArrow(GetWorld(), LineStart, LineEnd, 10.0f, ArrowColor, 3.0f, 2.0f);

		// TODO: Send event to launch EnemyActor
		FGameplayEventData EventData;
		URGX_LaunchEventPayload* LaunchPayloadObject = NewObject<URGX_LaunchEventPayload>();
		LaunchPayloadObject->LaunchHorizontalForce = 500.0f; //TODO: Do not hardcode this
		LaunchPayloadObject->LaunchVerticalForce = 500.0f;
		LaunchPayloadObject->bOverrideHorizontal = true;
		LaunchPayloadObject->bOverrideVertical = true;
		LaunchPayloadObject->ForceOrigin = SpawnLocation;

		EventData.OptionalObject = LaunchPayloadObject;

		Enemy->GetAbilitySystemComponent()->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Launched")), &EventData);
	}
}