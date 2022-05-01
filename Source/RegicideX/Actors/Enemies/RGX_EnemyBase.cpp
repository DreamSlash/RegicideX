// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_EnemyBase.h"
#include "Components/MCV_AbilitySystemComponent.h"
#include "RegicideX/GAS/AttributeSets/RGX_HealthAttributeSet.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
ARGX_EnemyBase::ARGX_EnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UMCV_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	HealthAttributeSet = CreateDefaultSubobject<URGX_HealthAttributeSet>(TEXT("HealthAttributeSet"));
}

// Called when the game starts or when spawned
void ARGX_EnemyBase::BeginPlay()
{
	Super::BeginPlay();


}

void ARGX_EnemyBase::MoveToTarget(float DeltaTime, FVector TargetPos)
{
	if (TargetActor)
	{
		const FVector MyFront = this->GetActorForwardVector();
		const FVector CurrentLocation = this->GetActorLocation();
		const FVector NewLocation = CurrentLocation + MyFront * MoveSpeed * DeltaTime;
		this->SetActorLocation(NewLocation);
	}
}

void ARGX_EnemyBase::RotateToTarget(float DeltaTime)
{
	if (TargetActor)
	{
		const FVector MyLocation = this->GetActorLocation();
		const FVector TargetLocation = TargetActor->GetActorLocation();
		const FRotator RotOffset = UKismetMathLibrary::FindLookAtRotation(MyLocation, TargetLocation);
		const FRotator NewRotation = FMath::Lerp(this->GetActorRotation(), RotOffset, DeltaTime * InterpSpeed);
		this->SetActorRotation(NewRotation);
	}
}

// Called every frame
void ARGX_EnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ARGX_EnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* ARGX_EnemyBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ARGX_EnemyBase::HandleDamage(FAttackInfo info)
{
}

void ARGX_EnemyBase::HandleDeath()
{
}

void ARGX_EnemyBase::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	CharacterTeam = TeamID;
}

FGenericTeamId ARGX_EnemyBase::GetGenericTeamId() const
{
	return CharacterTeam;
}

void ARGX_EnemyBase::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	AbilitySystemComponent->GetOwnedGameplayTags(TagContainer);
}

bool ARGX_EnemyBase::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	return AbilitySystemComponent->HasMatchingGameplayTag(TagToCheck);
}

bool ARGX_EnemyBase::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystemComponent->HasAllMatchingGameplayTags(TagContainer);
}

bool ARGX_EnemyBase::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystemComponent->HasAnyMatchingGameplayTags(TagContainer);
}

void ARGX_EnemyBase::AddGameplayTag(const FGameplayTag& TagToAdd)
{
	AbilitySystemComponent->AddLooseGameplayTag(TagToAdd);
}

void ARGX_EnemyBase::RemoveGameplayTag(const FGameplayTag& TagToRemove)
{
	AbilitySystemComponent->RemoveLooseGameplayTag(TagToRemove);
}

