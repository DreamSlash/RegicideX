
#include "RGX_EnemyBaseController.h"

#include "BehaviorTree/BlackboardComponent.h"

ARGX_EnemyBaseController::ARGX_EnemyBaseController()
{
	
}

void ARGX_EnemyBaseController::DamageTaken()
{
	if (Blackboard && Blackboard->GetBlackboardAsset())
	{
		const FName key = "ConsecutiveHits";
		int currentValue = Blackboard->GetValueAsInt(key);
		Blackboard->SetValueAsInt(key, ++currentValue);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() {
			if (Blackboard && Blackboard->GetBlackboardAsset())
			{
				const FName key = "ConsecutiveHits";
				Blackboard->SetValueAsInt(key, 0);
			}
		}, TimeConsecutiveHits, false);
	}
}

ERGX_EnemyAIState ARGX_EnemyBaseController::GetEnemyAIState() const
{
	if (Blackboard && Blackboard->GetBlackboardAsset())
	{
		return (ERGX_EnemyAIState)Blackboard->GetValueAsEnum("AIState");
	}

	return ERGX_EnemyAIState::None;
}

void ARGX_EnemyBaseController::SetEnemyAIState(ERGX_EnemyAIState state)
{
	if (Blackboard && Blackboard->GetBlackboardAsset())
	{
		Blackboard->SetValueAsEnum("AIState", state);
	}
}

bool ARGX_EnemyBaseController::InitializeBlackboard(UBlackboardComponent& BlackboardComp, UBlackboardData& BlackboardAsset)
{
	bool bResult = Super::InitializeBlackboard(BlackboardComp, BlackboardAsset);

	if (bResult)
	{
		BlackboardComp.SetValueAsEnum("AIState", ERGX_EnemyAIState::None);
	}

	return bResult;
}