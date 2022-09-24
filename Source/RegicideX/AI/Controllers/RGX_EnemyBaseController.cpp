
#include "RGX_EnemyBaseController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Rotator.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Class.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Name.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_String.h"
#include "BehaviorTree/BlackboardComponent.h"

ARGX_EnemyBaseController::ARGX_EnemyBaseController(const FObjectInitializer& ObjectInitializer)
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree Component"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));
}

void ARGX_EnemyBaseController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ARGX_EnemyBase* enemy = Cast<ARGX_EnemyBase>(InPawn);
	if (enemy != nullptr && enemy->BehaviorTree != nullptr)
	{
		// Initialize Blackboard
		BlackboardComponent->InitializeBlackboard(*enemy->BehaviorTree->BlackboardAsset);

		// Set Blackboard Key IDS
		TargetKeyId = BlackboardComponent->GetKeyID("TargetActor");
		DistanceToPlayerKeyId = BlackboardComponent->GetKeyID("DistanceToPlayer");
		RandomNumberKeyId = BlackboardComponent->GetKeyID("RandomNumber");
		AIStateKeyId = BlackboardComponent->GetKeyID("AIState");
		ConsecutiveHitsKeyId = BlackboardComponent->GetKeyID("ConsecutiveHits");

		// Start Behavior Tree
		BehaviorTreeComponent->StartTree(*enemy->BehaviorTree);
	}
}

void ARGX_EnemyBaseController::DamageTaken()
{
	if (Blackboard && Blackboard->GetBlackboardAsset())
	{
		int currentValue = Blackboard->GetValue<UBlackboardKeyType_Int>(ConsecutiveHitsKeyId);
		Blackboard->SetValue<UBlackboardKeyType_Int>(ConsecutiveHitsKeyId, ++currentValue);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() {
			if (Blackboard && Blackboard->GetBlackboardAsset())
			{
				Blackboard->SetValue<UBlackboardKeyType_Int>(ConsecutiveHitsKeyId, 0);
			}
		}, TimeConsecutiveHits, false);
	}
}

ERGX_EnemyAIState ARGX_EnemyBaseController::GetEnemyAIState() const
{
	if (Blackboard && Blackboard->GetBlackboardAsset())
	{
		return (ERGX_EnemyAIState)Blackboard->GetValue<UBlackboardKeyType_Enum>(AIStateKeyId);
	}

	return ERGX_EnemyAIState::None;
}

void ARGX_EnemyBaseController::SetEnemyAIState(ERGX_EnemyAIState state)
{
	if (Blackboard && Blackboard->GetBlackboardAsset())
	{
		Blackboard->SetValue<UBlackboardKeyType_Enum>(AIStateKeyId, state);
	}
}

bool ARGX_EnemyBaseController::InitializeBlackboard(UBlackboardComponent& BlackboardComp, UBlackboardData& BlackboardAsset)
{
	bool bResult = Super::InitializeBlackboard(BlackboardComp, BlackboardAsset);

	if (bResult)
	{
		if (ARGX_EnemyBase* enemy = GetPawn<ARGX_EnemyBase>())
		{
			BlackboardComp.SetValue<UBlackboardKeyType_Object>(TargetKeyId, enemy->TargetActor);
		}

		BlackboardComp.SetValue<UBlackboardKeyType_Enum>(AIStateKeyId, ERGX_EnemyAIState::None);
	}

	return bResult;
}