#include "RGX_GA_BarrierAbility.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "Components/MCV_AbilitySystemComponent.h"
#include "UObject/UObjectGlobals.h"
#include "RegicideX/Character/RGX_PlayerCharacter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

URGX_BarrierAbility::URGX_BarrierAbility()
{

}

bool URGX_BarrierAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (bResult == false)
		return false;

	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor);
	if (Character == nullptr)
		return false;

	return true;
}

void URGX_BarrierAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	FGameplayEventData EventData;
	CurrentActorInfo->AbilitySystemComponent->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Character.Interrupted")), &EventData);

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void URGX_BarrierAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URGX_BarrierAbility::OnReceivedEvent(FGameplayTag EventTag, FGameplayEventData EventData)
{
	FVector SpawnLocation = CurrentActorInfo->AvatarActor->GetActorLocation();
	FRotator SpawnRotation = FRotator(0.0f);

	AActor* SpawnedBarrier = GetWorld()->SpawnActor<AActor>(BarrierClass, SpawnLocation, SpawnRotation);

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	UClass* SeekClass = ARGX_EnemyBase::StaticClass();

	TArray<AActor*> IgnoreActors;
	TArray<AActor*> OutActors;

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), SpawnLocation, Radius, TraceObjectTypes, SeekClass, IgnoreActors, OutActors);

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

		FGameplayEventData EventData;
		EventData.Instigator = CurrentActorInfo->AvatarActor.Get();
		EventData.OptionalObject = LaunchEventData;

		Enemy->GetAbilitySystemComponent()->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Launched")), &EventData);
	}
}