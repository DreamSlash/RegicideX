// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvironmentQuery/EnvQuery.h"
#include "Kismet/GameplayStatics.h"
#include "RGX_GroupManager.h"
#include "RGX_Peasant.h"
#include "Math/NumericLimits.h"

// Sets default values
ARGX_GroupManager::ARGX_GroupManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//PeasantArray.AddUninitialized(MaxNumberOfPeasants);
}

// Called when the game starts or when spawned
void ARGX_GroupManager::BeginPlay()
{
	Super::BeginPlay();
	TargetActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	LastPlayerLocation = TargetActor->GetActorLocation();
	PositionQueryRequest = FEnvQueryRequest(PositionQuery, this);
	RecalcPositions();
}

// Called every frame
void ARGX_GroupManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARGX_GroupManager::AddPeasant(ARGX_Peasant* newPeasant)
{
	if (!newPeasant)
		return;

	if (PeasantArray.Num() == MaxNumberOfPeasants) {
		// ERROR! Array is full, should not add a peasant when full.
		UE_LOG(LogTemp, Error, TEXT("Cannot add a new peasant to the manager if full.\n"));
		return;
	}

	PeasantArray.Add(newPeasant);
	CurrentNumberOfPeasants++;
}

void ARGX_GroupManager::RemovePeasant(ARGX_Peasant* PeasantToRemove)
{
	PeasantArray.RemoveSingle(PeasantToRemove);
	CurrentNumberOfPeasants--;
	RecalcPeasants();
}

void ARGX_GroupManager::RecalcPositions()
{
	PositionQueryRequest.Execute(EEnvQueryRunMode::SingleResult, this, &ARGX_GroupManager::PositionEQSFinished);
	bCanSpawn = true;
}

void ARGX_GroupManager::PositionEQSFinished(TSharedPtr<FEnvQueryResult> Result)
{
	if (Result->IsSuccsessful())
	{
		Result->GetAllAsLocations(PositionsAroundTarget);
		//OnPeasantAdded();
	}
}

void ARGX_GroupManager::OnPeasantAdded()
{
	int AuxiliarIdx = 0;
	for (ARGX_Peasant* Peasant : PeasantArray)
	{
		float auxMinDistance = TNumericLimits<float>::Max();
		int Idx = 0;
		for (auto& Position : PositionsAroundTarget)
		{
			FVector Location = Peasant->GetActorLocation();
			float Distance = FVector::Distance(Position, Location);
			auxMinDistance = Distance < auxMinDistance ? Distance : auxMinDistance;
			AuxiliarIdx = Idx;
			Idx++;
		}

		PeasantToPosition.Add(Peasant, PositionsAroundTarget[AuxiliarIdx]);
		PositionsAroundTarget.RemoveSingle(PositionsAroundTarget[AuxiliarIdx]);
		Peasant->IdlePosition = *PeasantToPosition.Find(Peasant);

	}

	PeasantArray.Empty();
	RecalcPeasants();
}

void ARGX_GroupManager::RecalcPeasants()
{
	if (CurrentAttackers < MaxAttackers && PeasantToPosition.Num() > 0)
	{
		int AuxNearestPeasant = -1;
		TArray<int> AuxAlreadyChecked;
		TArray<float> AuxDistanceToPlayer;
		TArray<ARGX_Peasant*> AuxPeasantsNearPlayer;

		for (auto& pair : PeasantToPosition)
		{
			ARGX_Peasant* Peasant = pair.Key;
			float Distance = FVector::Distance(Peasant->GetActorLocation(), TargetActor->GetActorLocation());
			AuxDistanceToPlayer.Add(Distance);
		}

		int32 AttackersDifference = (int32)(MaxAttackers - CurrentAttackers);
		while (AuxPeasantsNearPlayer.Num() < AuxDistanceToPlayer.Num() 
			&& AuxPeasantsNearPlayer.Num() < AttackersDifference)
		{
			float AuxMinDistance = TNumericLimits<float>::Max();
			TArray<ARGX_Peasant*> PeasantKeys;
			int Idx = 0;
			for (float& DistanceToPlayer : AuxDistanceToPlayer)
			{
				if (DistanceToPlayer < AuxMinDistance && AuxAlreadyChecked.Find(Idx) < 0)
				{
					AuxMinDistance = DistanceToPlayer < AuxMinDistance ? DistanceToPlayer : AuxMinDistance;
					AuxNearestPeasant = Idx;
					Idx++;
				}
			}
			PeasantToPosition.GenerateKeyArray(PeasantKeys);
			ARGX_Peasant* Peasant = PeasantKeys[AuxNearestPeasant];
			AuxPeasantsNearPlayer.Add(Peasant);
			AuxAlreadyChecked.Add(AuxNearestPeasant);
		}

		for (ARGX_Peasant* Peasant : AuxPeasantsNearPlayer)
		{
			PositionsAroundTarget.Add(*PeasantToPosition.Find(Peasant));
			Peasant->bInCombat = true;
			PeasantToPosition.Remove(Peasant);
			PeasantsInCombat.Add(Peasant);
			CurrentAttackers++;
		}
	}
}
