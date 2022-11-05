// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "RGX_BTTask_FindTeleportLocation.generated.h"

/**
 * 
 */

UCLASS()
class REGICIDEX_API URGX_BT_FindTeleportLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	URGX_BT_FindTeleportLocation(const FObjectInitializer& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override; 

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	UEnvQuery* LocationSeekerQuery;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	float TeleportDistance = 1000.f;

private:
	void LocationSeekerQueryFinished(TSharedPtr<FEnvQueryResult> Result);

	void SetLocation(UBehaviorTreeComponent& OwnerComp, const FVector& location);

private:
	FEnvQueryRequest LocationSeekerQueryRequest;

	class ARGX_EnemyBaseController* Controller = nullptr;

};


















