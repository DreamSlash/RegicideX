
#include "RGX_PeasantShieldController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

#include "RegicideX/Actors/Enemies/RGX_Peasant.h"

ARGX_PeasantShieldController::ARGX_PeasantShieldController(const FObjectInitializer& ObjectInitializer)
	: ARGX_PeasantController(ObjectInitializer)
{

}

void ARGX_PeasantShieldController::OnShieldBroken()
{
	if (LogicAfterShieldBroken)
	{
		BehaviorTreeComponent->StopLogic("Shield Broken");

		InitializeBlackboard(*Blackboard, *LogicAfterShieldBroken->BlackboardAsset);
		BehaviorTreeComponent->StartTree(*LogicAfterShieldBroken);
	}
}