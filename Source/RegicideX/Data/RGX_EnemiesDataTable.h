#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RGX_EnemiesDataTable.generated.h"

USTRUCT(BlueprintType)
struct FRGX_EnemiesDataTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataAsset* EnemyInfo;


};
