#include "RGX_GA_SpearsAbility.h"
#include "Components/MCV_AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "RegicideX/Character/RGX_PlayerCharacter.h"
#include "RegicideX/Actors/RGX_SpearProjectile.h"
#include "RegicideX/GAS/RGX_GameplayEffectContext.h"
#include "RegicideX/Components/RGX_CameraControllerComponent.h"
#include "RegicideX/Components/RGX_HitboxComponent.h"
#include "Kismet/GameplayStatics.h"

bool URGX_SpearsAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void URGX_SpearsAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const bool bActivationSuccessful = CommitAbility(Handle, ActorInfo, ActivationInfo);

	if (bActivationSuccessful == false)
		return;
}

void URGX_SpearsAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URGX_SpearsAbility::CastSpearsAttack(AActor* CasterActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Cast Spears Attack\n"));

	if (CasterActor == nullptr)
		return;
	
	Caster = CasterActor;
	
	const float AnglePerSpear = SpearsSpawnAngle / (MaxNumSpears);

	for (int i = 0; i < MaxNumSpears; ++i)
	{
		const FVector CasterLocation = CasterActor->GetActorLocation();
		const FVector Right = CasterActor->GetActorRightVector();
		const FVector Forward = CasterActor->GetActorForwardVector();

		// Rotation around caster forward's vector
		const float SpearAngle = AnglePerSpear * i;
		const FVector SpearOffsetRotation = Right.RotateAngleAxis(SpearAngle, Forward);

		// Place the spear away from the caster and rotated around forward
		const FVector SpearOffset = DistanceFromCaster * SpearOffsetRotation;
		const FVector SpawnLocation = CasterActor->GetActorLocation() + SpearOffset;

		const FRotator SpawnRotation = CasterActor->GetActorRotation();
		
		
		if(ARGX_SpearProjectile* SpawnedSpear = GetWorld()->SpawnActor<ARGX_SpearProjectile>(SpearProjectileClass, SpawnLocation, SpawnRotation))
		{
			SpawnedSpear->Angle = SpearAngle;
			SpawnedSpear->Instigator = CasterActor;
			SpawnedSpear->SpawnIndex = i;

			//FGameplayEffectContextHandle ContextHandle = MakeEffectContext(GetCurrentAbilitySpecHandle(), CurrentActorInfo);
			//FRGX_GameplayEffectContext* Context = static_cast<FRGX_GameplayEffectContext*>(ContextHandle.Get());
			//Context->ScalingAttributeFactor = 1.0f;

			//URGX_HitboxComponent* HitboxComponent = SpawnedSpear->FindComponentByClass<URGX_HitboxComponent>();

			if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(CasterActor))
			{
				SpawnedSpear->SetGenericTeamId(TeamAgent->GetGenericTeamId());
			}

			//OnHoldSpearsTimeOut();
			SpearsArray.Add(SpawnedSpear);
			LaunchSpearsAttack();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Spear Not Casted\n"));
		}

	}

	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
	
}

void URGX_SpearsAbility::LaunchSpearsAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Launch Spears Attack\n"));

	ARGX_PlayerCharacter* Player = Cast<ARGX_PlayerCharacter>(Caster);
	if (!Player)
		return;

	URGX_CameraControllerComponent* CameraComponent = Player->FindComponentByClass<URGX_CameraControllerComponent>();
	if (!CameraComponent)
		return;

	AActor* Target = nullptr;
	if (CameraComponent->bTargetingEnabled) {
		Target = Cast<AActor>(CameraComponent->CurrentTarget);
	}
	else
	{
		TArray<AActor*> FoundEnemies;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARGX_EnemyBase::StaticClass(), FoundEnemies);

		float ClosestEnemyDistance = INFINITY;

		for (AActor* Enemy : FoundEnemies)
		{
			ARGX_EnemyBase* EnemyBase = Cast<ARGX_EnemyBase>(Enemy);
			bool bIsDead = EnemyBase->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.Dead")));
			if (bIsDead == true)
				continue;

			FVector CasterLocation = Caster->GetActorLocation();
			FVector EnemyLocation = EnemyBase->GetActorLocation();
			float Distance = FVector::Dist(CasterLocation, EnemyLocation);

			if (Distance < ClosestEnemyDistance)
			{
				Target = EnemyBase;
				ClosestEnemyDistance = Distance;
			}
		}
	}

	for (int i = 0; i < SpearsArray.Num(); ++i)
	{
		UE_LOG(LogTemp, Warning, TEXT("Launch Spears\n"));
		SpearsArray[i]->LaunchProjectile(Target);
	}

	SpearsArray.Empty();
}

void URGX_SpearsAbility::OnHoldSpearsTimeOut()
{
	LaunchSpearsAttack();
	UE_LOG(LogTemp, Warning, TEXT("On Hold Spears Time Out\n"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
