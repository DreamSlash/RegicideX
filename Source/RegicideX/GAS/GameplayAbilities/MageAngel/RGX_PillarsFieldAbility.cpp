#include "RGX_PillarsFieldAbility.h"

#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RegicideX/Actors/Weapons/RGX_ExplosivePillar.h"

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

void URGX_PillarsFieldAbility::OnAttackWindow()
{
	FOccluderVertexArray locations;
	PillarsPositionsResult->GetAllAsLocations(locations);

	for (const FVector& location : locations)
	{
		const FTransform pillarTransform(location);
		GetWorld()->SpawnActor<ARGX_ExplosivePillar>(PillarActorClass, pillarTransform);
	}
}
