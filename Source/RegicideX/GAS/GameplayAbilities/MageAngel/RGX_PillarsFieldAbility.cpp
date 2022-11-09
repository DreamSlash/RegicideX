#include "RGX_PillarsFieldAbility.h"

#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RegicideX/Actors/Weapons/RGX_ExplosiveMine.h"

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

void URGX_PillarsFieldAbility::OnGround()
{
	Super::OnGround();

	AActor* avatarActor = GetAvatarActorFromActorInfo();
	APawn* enemy = Cast<APawn>(avatarActor);

	FOccluderVertexArray locations;
	PillarsPositionsResult->GetAllAsLocations(locations);

	for (const FVector& location : locations)
	{
		FTransform pillarTransform(location);

		FHitResult Result;
		FVector endLocation = location; endLocation.Z = -100.f;
		if (GetWorld()->LineTraceSingleByChannel(Result, location, endLocation, ECollisionChannel::ECC_GameTraceChannel17))
		{
			FVector newLocation = location;
			newLocation.Z = Result.ImpactPoint.Z;
			pillarTransform.SetLocation(newLocation);
		}

		FActorSpawnParameters params;
		params.Instigator = enemy;
		//FActorSpawnParameters params;
		/*params.Owner = mageAngel;
		params.Instigator = mageAngel;*/
		//params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		if (ARGX_ExplosiveMine* mine = GetWorld()->SpawnActor<ARGX_ExplosiveMine>(PillarActorClass, pillarTransform, params))
		{
			if (ARGX_EnemyBase* enemyBase = Cast<ARGX_EnemyBase>(avatarActor))
			{
				mine->SetTargetActor(enemyBase->TargetActor);
			}
		}
	}
}
