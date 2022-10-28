// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_GA_ClusteredProjectiles.h"
#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "RegicideX\Actors\Enemies\RGX_Peasant.h"
#include "RegicideX/Actors/Projectiles/RGX_Projectile.h"

void URGX_GA_ClusteredProjectiles::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{

	// Stop focusing while shooting
	ARGX_Peasant* Peasant		= Cast<ARGX_Peasant>(ActorInfo->AvatarActor);
	AAIController* Controller = Cast<AAIController>(Peasant->GetController());
	if (Controller)
	{
		Controller->SetFocus(nullptr);	// Stop focusing the player
	}

	const FVector Forward		= Peasant->GetActorForwardVector();
	const FVector Right			= Peasant->GetActorRightVector();
	const FVector Up			= Peasant->GetActorUpVector();
	const FVector Center		= Peasant->GetActorLocation();
	Transform					= FTransform(Forward.Rotation(), Center, FVector(1.0));

	TeamIdToApply				= Peasant->GetGenericTeamId();
	
	for (int i = 0; i < NumberProjectilesToFire; ++i)
	{
		FVector2D Point2D = FMath::RandPointInCircle(500.0f);
		FVector Position;
		Position.X = Right.X * Point2D.X;
		Position.Y = Right.Y * Point2D.Y;
		Position.Z = FMath::RandRange(20.0f, 100.0f);
		Position = Center + Position;
		PointsToSpawn.Add(Position);
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	FGameplayTag EventTag;
	FGameplayEventData EventData;
	OnReceivedEvent(EventTag, EventData);
}

void URGX_GA_ClusteredProjectiles::OnReceivedEvent(
	FGameplayTag EventTag, 
	FGameplayEventData EventData)
{
	for (const auto& Point : PointsToSpawn)
	{
		Transform.SetTranslation(Point);
		ARGX_Projectile* Bullet = GetWorld()->SpawnActor<ARGX_Projectile>(BulletBP, Transform);
		if (!Bullet)
			continue;
		Bullet->SetGenericTeamId(TeamIdToApply);
		Bullet->Instigator = GetAvatarActorFromActorInfo();
	}
	PointsToSpawn.Empty();
}

void URGX_GA_ClusteredProjectiles::EndAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	bool bReplicateEndAbility, 
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	// Stop focusing while shooting
	ACharacter* Character = Cast<ACharacter>(ActorInfo->OwnerActor);
	AAIController* Controller = Cast<AAIController>(Character->GetController());
	ARGX_EnemyBase* Self = Cast<ARGX_EnemyBase>(Character);
	if (Controller)
	{
		Controller->SetFocus(Self->TargetActor);	// Stop focusing the player
	}
}
