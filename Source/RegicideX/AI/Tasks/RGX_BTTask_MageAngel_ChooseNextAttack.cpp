#include "RGX_BTTask_MageAngel_ChooseNextAttack.h"

#include "AIController.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"

#pragma optimize("", off)

EBTNodeResult::Type URGX_BTTask_MageAngel_ChooseNextAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* aiController = OwnerComp.GetAIOwner();
	ARGX_EnemyBase* enemy = Cast<ARGX_EnemyBase>(aiController->GetPawn());
	const AActor* target = enemy->TargetActor;

	constexpr int32 PillarsAttack = 0;
	constexpr int32 RingWavesAttack = 1;
	constexpr int32 GroundExplosionAttack = 2;

	constexpr float DangerRadius = 1000.0f;
	const float distance = FVector::Distance(target->GetActorLocation(), enemy->GetActorLocation());
	if (distance > DangerRadius)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(BlackboardKey.SelectedKeyName, GroundExplosionAttack);
		return EBTNodeResult::Succeeded;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsInt(BlackboardKey.SelectedKeyName, RingWavesAttack);
	return EBTNodeResult::Succeeded;

	const FVector forward = target->GetActorForwardVector();
	const FVector targetDirectionFromEnemy = target->GetActorLocation() - enemy->GetActorLocation();
	const float dot = FVector::DotProduct(targetDirectionFromEnemy, forward);
	if (dot < -0.8)
	{

	}

	return EBTNodeResult::Succeeded;
}