// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_EnemyBase.h"
#include "Components/MCV_AbilitySystemComponent.h"

// Sets default values
ARGX_EnemyBase::ARGX_EnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	AbilitySystemComponent = CreateDefaultSubobject<UMCV_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

// Called when the game starts or when spawned
void ARGX_EnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
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

