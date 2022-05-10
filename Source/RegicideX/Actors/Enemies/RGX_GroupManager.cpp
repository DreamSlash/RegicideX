// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_GroupManager.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "Kismet/GameplayStatics.h"
#include "RGX_Peasant.h"
#include "Math/NumericLimits.h"

// Sets default values
ARGX_GroupManager::ARGX_GroupManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARGX_GroupManager::BeginPlay()
{
	Super::BeginPlay();

	TargetActor				= UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	LastPlayerLocation		= TargetActor->GetActorLocation();
	PositionQueryRequest	= FEnvQueryRequest(PositionQuery, TargetActor);

	RecalcPositions();	// Execute EQS to receive Peasant Idle positions.
}

// Called every frame
void ARGX_GroupManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FVector::Distance(LastPlayerLocation, TargetActor->GetActorLocation()) > OffsetPlayer)
	{
		Invalidate();
		RecalcPositions();
	}
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

void ARGX_GroupManager::Invalidate()
{
	for (auto& pair : PeasantToPosition)
	{
		ARGX_Peasant* key = pair.Key;
		PeasantArray.Add(key);
	}

	PeasantToPosition.Empty();

	for (ARGX_Peasant* Peasant : PeasantsInCombat)
	{
		Peasant->bInCombat = false;
		PeasantArray.Add(Peasant);
	}
	PeasantsInCombat.Empty();
	CurrentAttackers = 0;
	LastPlayerLocation = TargetActor->GetActorLocation();
}

void ARGX_GroupManager::PositionEQSFinished(TSharedPtr<FEnvQueryResult> Result)
{
	if (Result->IsSuccsessful())
	{
		// If successful, get all position into the vector.
		Result->GetAllAsLocations(PositionsAroundTarget);
		for (FVector& Value : PositionsAroundTarget)
		{
			Value.Z = 0.0f;
		}
		UE_LOG(LogTemp, Warning, TEXT("The position is ... %s ."), *PositionsAroundTarget[0].ToString());
		OnPeasantAdded();
	}
}

// Assign all peasants from PeasantArray to PeasantToPosition with the closest
// idle position to him. After PeasantArray has been emptied and all peasants have
// an idle position assigned, we call RecalcPeasants to assign attackers ...
void ARGX_GroupManager::OnPeasantAdded()
{
	int AuxiliarIdx = 0;
	for (ARGX_Peasant* Peasant : PeasantArray)
	{
		float auxMinDistance = TNumericLimits<float>::Max();
		int Idx = 0;
		for (auto& Position : PositionsAroundTarget)
		{
			FVector Location	= Peasant->GetActorLocation();
			float Distance		= FVector::Distance(Position, Location);
			if (Distance < auxMinDistance)
			{
				auxMinDistance	= Distance;
				AuxiliarIdx		= Idx;
			}
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
	// If we have peasants and not all attackers assigned, execute ...
	if (CurrentAttackers < MaxAttackers && PeasantToPosition.Num() > 0)
	{
		TArray<ARGX_Peasant*> AuxPeasantsNearPlayer;
		TMap<ARGX_Peasant*, float> PeasantToDistance;

		// Save all distances to target
		for (auto& pair : PeasantToPosition)
		{
			ARGX_Peasant* Key = pair.Key;
			float Distance = FVector::Distance(Key->GetActorLocation(), TargetActor->GetActorLocation());
			PeasantToDistance.Add(Key, Distance);
		}

		// Sort from nearest to farthest
		PeasantToDistance.ValueSort([](const float& a, const float b) {
			return a < b;
			});
		
		TArray<ARGX_Peasant*> PeasantsToAttack;
		PeasantToDistance.GenerateKeyArray(PeasantsToAttack);

		const unsigned int NumAttackers = std::min(PeasantToDistance.Num(), (int)MaxAttackers);
		for (unsigned int i = 0; i < NumAttackers; ++i)
		{
			ARGX_Peasant* p = PeasantsToAttack[i];
			p->bInCombat = true;
			PeasantToPosition.Remove(p);
			PeasantsInCombat.Add(p);
			CurrentAttackers++;
		}
	}
}
