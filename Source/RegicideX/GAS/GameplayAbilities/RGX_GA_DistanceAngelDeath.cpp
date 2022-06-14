// Fill out your copyright notice in the Description page of Project Settings.

#include "RGX_GA_DistanceAngelDeath.h"
#include "RegicideX\Actors\Enemies\RGX_DistanceAngel.h"
#include "RegicideX\AI\Controllers\RGX_DistanceAngelController.h"
#include "BrainComponent.h"
#include "Components/SphereComponent.h"

void URGX_GA_DistanceAngelDeath::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ARGX_DistanceAngel* DistAngel = Cast<ARGX_DistanceAngel>(ActorInfo->AvatarActor);

	//stop Logic

	ARGX_DistanceAngelController* Controller = Cast< ARGX_DistanceAngelController>(DistAngel->GetController());
	UBrainComponent* BrainComponent = Controller->GetBrainComponent();
	BrainComponent->StopLogic("");

	DistAngel->SphereCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if(DistAngel->LaserBeamRef != nullptr)
	{
		DistAngel->LaserBeamRef->Destroy();
	}

	DistAngel->Ring_1_Mesh->SetSimulatePhysics(true);
	DistAngel->Ring_2_Mesh->SetSimulatePhysics(true);
	DistAngel->Ring_3_Mesh->SetSimulatePhysics(true);
	DistAngel->PrimaryActorTick.bCanEverTick = false;

	DistAngel->DestroyMyself(22.0f);

	

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}
