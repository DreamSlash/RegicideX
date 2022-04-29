#include "RGX_GA_SpearsAbility.h"
#include "GameFramework/Character.h"
#include "../../Actors/RGX_SpearProjectile.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Components/MCV_AbilitySystemComponent.h"
#include "GenericTeamAgentInterface.h"

/*
void URGX_SpearsAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//AActor* CasterActor = ActorInfo->AvatarActor.Get();
	//CastSpearsAttack(CasterActor);
}
*/

void URGX_SpearsAbility::OnInputReleased(float TimeHeld)
{
	LaunchSpearsAttack();
	UE_LOG(LogTemp, Warning, TEXT("On Released\n"));
}

void URGX_SpearsAbility::CastSpearsAttack(AActor* CasterActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Cast Spears Attack\n"));
	
	float AnglePerSpear = SpearsSpawnAngle / (MaxNumSpears - 1);

	for (int i = 0; i < MaxNumSpears - 1; ++i)
	{
		FVector CasterLocation = CasterActor->GetActorLocation();
		FVector Right = CasterActor->GetActorRightVector();
		FVector Forward = CasterActor->GetActorForwardVector();

		// Rotation around caster forward's vector
		float SpearAngle = AnglePerSpear * i;
		FVector SpearOffsetRotation = Right.RotateAngleAxis(SpearAngle, Forward);

		// Place the spear away from the caster and rotated around forward
		FVector SpawnLocation = CasterActor->GetActorLocation();
		FVector SpearOffset = DistanceFromCaster * SpearOffsetRotation;
		SpawnLocation += SpearOffset;

		FRotator SpawnRotation = CasterActor->GetActorRotation();

		ARGX_SpearProjectile* SpawnedSpear = GetWorld()->SpawnActor<ARGX_SpearProjectile>(SpearProjectileClass, SpawnLocation, SpawnRotation);
		SpawnedSpear->Angle = SpearAngle;
		SpawnedSpear->Caster = CasterActor;

		IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(CasterActor);

		if (TeamAgent != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Not null manuela\n"));
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
