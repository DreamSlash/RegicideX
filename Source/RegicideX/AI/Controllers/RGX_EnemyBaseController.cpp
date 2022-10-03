
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

#include "Kismet/GameplayStatics.h"

#include "Navigation/CrowdFollowingComponent.h"

#include "RegicideX/Actors/RGX_CombatManager.h"

//#pragma optimize("", off)

ARGX_EnemyBaseController::ARGX_EnemyBaseController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree Component"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));
}

void ARGX_EnemyBaseController::BeginPlay()
{
	Super::BeginPlay();

	CombatManager = Cast<ARGX_CombatManager>(UGameplayStatics::GetActorOfClass(this, ARGX_CombatManager::StaticClass()));

	//TargetActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

void ARGX_EnemyBaseController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ARGX_EnemyBase* enemy = Cast<ARGX_EnemyBase>(InPawn);
	if (enemy != nullptr && enemy->BehaviorTree != nullptr)
	{
		Agent = enemy;

		// Initialize Blackboard
		InitializeBlackboard(*Blackboard, *enemy->BehaviorTree->BlackboardAsset);
		//BlackboardComponent->InitializeBlackboard(*enemy->BehaviorTree->BlackboardAsset);
	}
}

void ARGX_EnemyBaseController::StartLogic()
{
	// Start Behavior Tree
	BehaviorTreeComponent->StartTree(*Agent->BehaviorTree);
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

ERGX_EnemyAIState::Type ARGX_EnemyBaseController::GetEnemyAIState() const
{
	if (Blackboard && Blackboard->GetBlackboardAsset())
	{
		return (ERGX_EnemyAIState::Type)Blackboard->GetValue<UBlackboardKeyType_Enum>(AIStateKeyId);
	}

	return ERGX_EnemyAIState::None;
}

void ARGX_EnemyBaseController::SetEnemyAIState(ERGX_EnemyAIState::Type state)
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
		// Set Blackboard Key IDS
		TargetKeyId = Blackboard->GetKeyID("TargetActor");
		DistanceToPlayerKeyId = Blackboard->GetKeyID("DistanceToPlayer");
		RandomNumberKeyId = Blackboard->GetKeyID("RandomNumber");
		AIStateKeyId = Blackboard->GetKeyID("AIState");
		ConsecutiveHitsKeyId = Blackboard->GetKeyID("ConsecutiveHits");
		StrafeDirectionKeyId = Blackboard->GetKeyID("StrafeDirection");
		StrafeLocationKeyId = Blackboard->GetKeyID("StrafeLocation");

		BlackboardComp.SetValue<UBlackboardKeyType_Object>(TargetKeyId, Agent->TargetActor);
		BlackboardComp.SetValue<UBlackboardKeyType_Enum>(AIStateKeyId, ERGX_EnemyAIState::None);
		BlackboardComp.SetValue<UBlackboardKeyType_Enum>(StrafeDirectionKeyId, ERGX_StrafeDirection::None);
	}

	return bResult;
}