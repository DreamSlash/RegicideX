#include "RGX_GA_SpearsAbility.h"
#include "GameFramework/Character.h"
#include "../../Actors/RGX_SpearProjectile.h"
#include "Components/MCV_AbilitySystemComponent.h"
#include "GenericTeamAgentInterface.h"

void URGX_SpearsAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const bool bActivationSuccessful = CommitAbility(Handle, ActorInfo, ActivationInfo);

	if (bActivationSuccessful == false)
		return;

	CastSpearsAttack(ActorInfo->AvatarActor.Get());

	WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this);
	WaitInputReleaseTask->OnRelease.AddDynamic(this, &URGX_SpearsAbility::OnInputReleased);
	WaitInputReleaseTask->ReadyForActivation();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &URGX_SpearsAbility::OnHoldSpearsTimeOut, MaxHoldSpearsTime, false);
}

void URGX_SpearsAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URGX_SpearsAbility::OnInputReleased(float TimeHeld)
{
	LaunchSpearsAttack();
	UE_LOG(LogTemp, Warning, TEXT("On Released\n"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void URGX_SpearsAbility::CastSpearsAttack(AActor* CasterActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Cast Spears Attack\n"));
	
	const float AnglePerSpear = SpearsSpawnAngle / (MaxNumSpears - 1);

	for (int i = 0; i < MaxNumSpears - 1; ++i)
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

		ARGX_SpearProjectile* SpawnedSpear = GetWorld()->SpawnActor<ARGX_SpearProjectile>(SpearProjectileClass, SpawnLocation, SpawnRotation);
		SpawnedSpear->Angle = SpearAngle;
		SpawnedSpear->Caster = CasterActor;

		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(CasterActor))
		{
			SpawnedSpear->CharacterTeam = TeamAgent->GetGenericTeamId();
		}

		SpearsArray.Add(SpawnedSpear);
	}
}

void URGX_SpearsAbility::LaunchSpearsAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Launch Spears Attack\n"));

	for (int i = 0; i < SpearsArray.Num(); ++i)
	{
		UE_LOG(LogTemp, Warning, TEXT("Launch Spears\n"));
		SpearsArray[i]->LaunchProjectile();
	}

	SpearsArray.Empty();
}

void URGX_SpearsAbility::OnHoldSpearsTimeOut()
{
	LaunchSpearsAttack();
	UE_LOG(LogTemp, Warning, TEXT("On Hold Spears Time Out\n"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
