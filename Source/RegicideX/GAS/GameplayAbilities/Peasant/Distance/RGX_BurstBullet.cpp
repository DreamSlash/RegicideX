
#include "RGX_BurstBullet.h"
#include "AIController.h"
#include "RegicideX\Actors\Enemies\RGX_Peasant.h"
#include "RegicideX/Actors/Projectiles/RGX_SimpleProjectile.h"
#include "Kismet/KismetMathLibrary.h"

void URGX_BurstBullet::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{

	const ARGX_Peasant* Self = Cast<ARGX_Peasant>(ActorInfo->AvatarActor);
	Character = Cast<ACharacter>(ActorInfo->AvatarActor);
	if (Self)
	{
		TeamIdToApply = Self->GetGenericTeamId();
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void URGX_BurstBullet::OnReceivedEvent(FGameplayTag EventTag, FGameplayEventData EventData)
{
	const ARGX_Peasant* Self	= Cast<ARGX_Peasant>(Character);
	const FVector Location		= Self->GetActorLocation();
	const FVector Direction		= UKismetMathLibrary::GetDirectionUnitVector(Location, Self->TargetActor->GetActorLocation());
	Transform					= FTransform(Direction.Rotation(), Location, FVector(1.0f));

	Counter++;
	SpawnBullet();
	if (Counter > NumberProjectilesToFire && Character)
	{
		Counter = 0;
		//Character->PlayAnimMontage(MontageToPlay, 1.0f, FName("EndBurstShoot"));
		MontageJumpToSection(FName("EndBurstShoot"));
	}
}

void URGX_BurstBullet::EndAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	bool bReplicateEndAbility, 
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URGX_BurstBullet::SpawnBullet()
{
	ARGX_Projectile* Bullet = GetWorld()->SpawnActor<ARGX_Projectile>(BulletBP, Transform);
	if(Bullet)
	{
		Bullet->SetGenericTeamId(TeamIdToApply);
		Bullet->Instigator = GetAvatarActorFromActorInfo();
	}
}
