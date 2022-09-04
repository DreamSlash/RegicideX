// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/GameplayFramework/RGX_LinearArenasGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "RegicideX/Actors/RGX_Arena.h"
#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RegicideX/Data/RGX_EnemiesDataTable.h"

void ARGX_LinearArenasGameMode::StartPlay()
{
	Super::StartPlay();
}

void ARGX_LinearArenasGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARGX_Arena::StaticClass(), Actors);
	for (AActor* Actor : Actors)
	{
		ARGX_Arena* Arena = Cast<ARGX_Arena>(Actor);
		if (Arena)
		{
			Arenas.Add(Arena);
			Arena->OnArenaActivated.AddDynamic(this, &ARGX_LinearArenasGameMode::OnArenaActivated);
			Arena->OnArenaDeactivated.AddDynamic(this, &ARGX_LinearArenasGameMode::OnArenaDeactivated);
			Arena->OnArenaEnemyKilled.AddDynamic(this, &ARGX_LinearArenasGameMode::OnArenaEnemyKilled);
		}
	}
}

void ARGX_LinearArenasGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	for (ARGX_Arena* Arena : Arenas)
	{
		Arena->OnArenaActivated.RemoveDynamic(this, &ARGX_LinearArenasGameMode::OnArenaActivated);
		Arena->OnArenaDeactivated.RemoveDynamic(this, &ARGX_LinearArenasGameMode::OnArenaDeactivated);
		Arena->OnArenaEnemyKilled.RemoveDynamic(this, &ARGX_LinearArenasGameMode::OnArenaEnemyKilled);
	}
}

void ARGX_LinearArenasGameMode::OnArenaActivated(ARGX_Arena* ActivatedArena)
{
	CurrentArena = ActivatedArena;
	BP_OnArenaActivated(ActivatedArena);
}

void ARGX_LinearArenasGameMode::OnArenaDeactivated(ARGX_Arena* DeactivatedArena)
{
	CurrentArena = nullptr;
	BP_OnArenaDeactivated(DeactivatedArena);

	if(DeactivatedArena->GetIsFinalArena() == true)
	{
		BP_OnPlayerWins();
	}
}

void ARGX_LinearArenasGameMode::OnArenaEnemyKilled(ARGX_EnemyBase* EnemyKilled)
{
	UE_LOG(LogTemp, Warning, TEXT("On Arena Enemy Killed"));
	ERGX_EnemyType EnemyType = EnemyKilled->GetEnemyType();

	// TODO: Use map instead of table?
	FName RowName = UEnum::GetValueAsName(EnemyType);
	FRGX_EnemyTypeDataTable* EnemyTypeRow = DT_EnemyTypesInfo->FindRow<FRGX_EnemyTypeDataTable>(RowName, "");
	if (EnemyTypeRow)
	{
		const int ScoreToAdd = EnemyTypeRow->BaseScore;
		Score += ScoreToAdd;
		BP_OnArenaEnemyKilled();
	}
}
