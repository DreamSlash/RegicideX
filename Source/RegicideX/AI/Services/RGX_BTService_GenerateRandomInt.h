// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "RGX_BTService_GenerateRandomInt.generated.h"

UCLASS()
class REGICIDEX_API URGX_BTService_GenerateRandomInt : public UBTService
{
	GENERATED_BODY()

public:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	void InitializeFromAsset(UBehaviorTree& Asset) override;

protected:
	UPROPERTY(EditAnywhere)
	int32 MinNumber = 0;

	UPROPERTY(EditAnywhere)
	int32 MaxNumber = 100;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector BlackboardKey;

private:
	bool SetNumberInBlackboard(UBehaviorTreeComponent& OwnerComp, int32 RandomNumber);

};
