// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_TargetQueryContext.h"

#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

#include "RegicideX/AI/Controllers/RGX_EnemyBaseController.h"

void URGX_TargetQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	if (ARGX_EnemyBaseController* controller = Cast<ARGX_EnemyBaseController>((Cast<AActor>((QueryInstance.Owner).Get())->GetInstigatorController())))
	{
		if (AActor* Target = Cast<AActor>(controller->BlackboardComponent->GetValue<UBlackboardKeyType_Object>(controller->TargetKeyId)))
		{
			UEnvQueryItemType_Actor::SetContextHelper(ContextData, Target);
		}
	}
}