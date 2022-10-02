#include "RGX_PillarsFieldAbility.h"

#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RegicideX/Actors/Weapons/RGX_ExplosivePillar.h"

#include "DrawDebugHelpers.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "SceneManagement.h"

void URGX_PillarsFieldAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	const ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(AvatarActor);
	AActor* Player = Enemy->TargetActor;

	FEnvQueryRequest PillarsPositionsRequest = FEnvQueryRequest(PillarsPositionsQuery, AvatarActor);
	PillarsPositionsRequest.Execute(EEnvQueryRunMode::SingleResult, this, &URGX_PillarsFieldAbility::PillarsPositionsQueryFinished);

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void URGX_PillarsFieldAbility::PillarsPositionsQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	PillarsPositionsResult = Result;
}

void URGX_PillarsFieldAbility::OnStartLoop()
{
	Super::OnStartLoop();

	FOccluderVertexArray locations;
	PillarsPositionsResult->GetAllAsLocations(locations);

	for (const FVector& location : locations)
	{
		FTransform pillarTransform(location);

		FHitResult Result;
		//DrawDebugLine(GetWorld(), location, location * FVector(1, 1, -1), FColor(255, 0, 0), true, 5.0f, 0, 5.0f);
		if (GetWorld()->LineTraceSingleByChannel(Result, location, location * FVector(1,1,-1), ECollisionChannel::ECC_WorldStatic))
		{
			FVector newLocation = location;
			newLocation.Z = Result.ImpactPoint.Z;
			pillarTransform.SetLocation(newLocation);
		}

		FActorSpawnParameters params;
		/*params.Owner = mageAngel;
		params.Instigator = mageAngel;*/
		//params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		GetWorld()->SpawnActor<ARGX_ExplosivePillar>(PillarActorClass, pillarTransform, params);
	}
}
