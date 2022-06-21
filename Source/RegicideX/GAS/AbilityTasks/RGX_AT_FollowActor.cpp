// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/GAS/AbilityTasks/RGX_AT_FollowActor.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

URGX_AT_FollowActor::URGX_AT_FollowActor()
{
	bTickingTask = true;
	bSimulatedTask = true;
	bIsFinished = false;
}

void URGX_AT_FollowActor::InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent)
{
	Super::InitSimulatedTask(InGameplayTasksComponent);

	TimeFollowStarted = GetWorld()->GetTimeSeconds();
	TimeFollowWillEnd = TimeFollowStarted + FollowDuration;
}

URGX_AT_FollowActor* URGX_AT_FollowActor::FollowActor(UGameplayAbility* OwningAbility, FName TaskInstanceName, AActor* ActorToFollow, float Speed, float FollowDuration)
{
	URGX_AT_FollowActor* MyObj = NewAbilityTask<URGX_AT_FollowActor>(OwningAbility, TaskInstanceName);

	MyObj->ActorToFollow = ActorToFollow;
	MyObj->FollowSpeed = Speed;
	MyObj->FollowDuration = FollowDuration;

	return MyObj;
}

void URGX_AT_FollowActor::Activate()
{
	TimeFollowStarted = GetWorld()->GetTimeSeconds();
	TimeFollowWillEnd = TimeFollowStarted + FollowDuration;
}

void URGX_AT_FollowActor::TickTask(float DeltaTime)
{
	if (bIsFinished)
	{
		return;
	}

	Super::TickTask(DeltaTime);

	AActor* MyActor = GetAvatarActor();
	if (MyActor)
	{
		float CurrentTime = GetWorld()->GetTimeSeconds();
		if (CurrentTime >= TimeFollowWillEnd)
		{
			bIsFinished = true;

			if (ShouldBroadcastAbilityTaskDelegates())
			{
				OnFollowActorEnded.Broadcast();
			}

			EndTask();
		}
		else
		{
			ACharacter* MyCharacter = Cast<ACharacter>(MyActor);
			if (MyCharacter)
			{

				UCharacterMovementComponent* CharacterMovementComponent = MyCharacter->GetCharacterMovement();
				if (CharacterMovementComponent)
				{
					AAIController* AIController = Cast<AAIController>(MyCharacter->GetController());
					if (AIController)
					{
						AIController->MoveToLocation(ActorToFollow->GetActorLocation());
					}
				}
			}
		}
	}
	else
	{
		bIsFinished = true;
		EndTask();
	}
}

void URGX_AT_FollowActor::OnDestroy(bool AbilityIsEnding)
{
	Super::OnDestroy(AbilityIsEnding);
}
