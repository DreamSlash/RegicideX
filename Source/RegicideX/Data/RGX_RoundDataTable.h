#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RGX_EnemiesDataTable.h"
#include "Kismet/GameplayStatics.h"

#include "RGX_RoundDataTable.generated.h"


USTRUCT(BlueprintType)
struct FRGX_RoundDataTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int> EnemiesToSpawn;

	FRGX_RoundDataTable();

	
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override;
};