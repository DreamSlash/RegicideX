#include "RGX_RoundDataTable.h"

FRGX_RoundDataTable::FRGX_RoundDataTable()
{
	EnemiesToSpawn.Init(0,4);
}

void FRGX_RoundDataTable::OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName)
{
	
	FTableRowBase::OnDataTableChanged(InDataTable, InRowName);
}
