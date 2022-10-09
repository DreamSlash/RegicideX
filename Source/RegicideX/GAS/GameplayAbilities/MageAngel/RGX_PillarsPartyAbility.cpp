#include "RGX_PillarsPartyAbility.h"

#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RegicideX/Actors/Weapons/RGX_ExplosivePillar.h"

#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"
#include "SceneManagement.h"

void URGX_PillarsPartyAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void URGX_PillarsPartyAbility::PositionsQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	PositionsQueryResult = Result;

	TArray<FVector> locations;
	PositionsQueryResult->GetAllAsLocations(locations);

	int32 index = 0;
	while (locations.Num() && index < NumPillars)
	{
		int32 i = FMath::RandRange(0, locations.Num()-1);
		SpawnPillarAtLocation(locations[i]);
		locations.RemoveAt(i);
		++index;
	}
}

void URGX_PillarsPartyAbility::OnGround()
{
	Super::OnGround();

	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	const ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(AvatarActor);
	AActor* Player = Enemy->TargetActor;

	PositionsQueryRequest = FEnvQueryRequest(PositionsQuery, AvatarActor);
	PositionsQueryRequest.SetFloatParam("SimpleGrid.GridSize", GridSize);
	PositionsQueryRequest.SetFloatParam("SimpleGrid.SpaceBetween", MinSpacingBetweenPillars);
	PositionsQueryRequest.SetFloatParam("Distance.FloatValueMin", MinDistanceFromPlayer);
	PositionsQueryRequest.SetFloatParam("Distance.FloatValueMax", MaxDistanceFromPlayer);
	PositionsQueryRequest.Execute(EEnvQueryRunMode::AllMatching, this, &URGX_PillarsPartyAbility::PositionsQueryFinished);

	/*PendingPillars = NumPillars;

	OnSpawnPillar();*/
}

void URGX_PillarsPartyAbility::SpawnPillarAtLocation(const FVector& Location) const
{
	FTransform pillarTransform(Location);

	FHitResult Result;
	if (GetWorld()->LineTraceSingleByChannel(Result, Location, Location * FVector(1, 1, -1), ECollisionChannel::ECC_WorldStatic))
	{
		FVector newLocation = Location;
		newLocation.Z = Result.ImpactPoint.Z;
		pillarTransform.SetLocation(newLocation);
	}

	FActorSpawnParameters params;
	GetWorld()->SpawnActor<ARGX_ExplosivePillar>(PillarActorClass, pillarTransform, params);
}

//void URGX_PillarsPartyAbility::OnSpawnPillar()
//{
//	const AActor* AvatarActor = GetAvatarActorFromActorInfo();
//	const ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(AvatarActor);
//	const AActor* Player = Enemy->TargetActor;
//
//	const FVector playerLocation = Player->GetActorLocation();
//	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetAvatarActorFromActorInfo());
//	
//	FNavLocation pillarLocation;
//	if (NavSystem->GetRandomReachablePointInRadius(playerLocation, PartyRadius, pillarLocation))
//	{
//		const FVector finalLocation = pillarLocation.Location;
//		FTransform pillarTransform(finalLocation);
//
//		FHitResult Result;
//		//DrawDebugLine(GetWorld(), finalLocation, finalLocation * FVector(1, 1, -1), FColor(255, 0, 0), true, 5.0f, 0, 5.0f);
//		if (GetWorld()->LineTraceSingleByChannel(Result, finalLocation, finalLocation * FVector(1, 1, -1), ECollisionChannel::ECC_WorldStatic))
//		{
//			FVector newLocation = finalLocation;
//			newLocation.Z = Result.ImpactPoint.Z;
//			pillarTransform.SetLocation(newLocation);
//		}
//
//		FActorSpawnParameters params;
//		GetWorld()->SpawnActor<ARGX_ExplosivePillar>(PillarActorClass, pillarTransform, params);
//	}
//
//	if (--PendingPillars > 0)
//	{
//		StartDelay();
//	}
//	else
//	{
//		bEndAttackingLoop = true;
//	}
//}
//
//void URGX_PillarsPartyAbility::StartDelay()
//{
//	GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, [this]() { OnSpawnPillar(); }, DelayBetweenPillars, false);
//}
