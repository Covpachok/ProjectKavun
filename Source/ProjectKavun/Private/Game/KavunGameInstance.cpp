


#include "Game/KavunGameInstance.h"

ULootTable* UKavunGameInstance::GetLootTable(ELootTableType Type)
{
	if(!LootTables.Contains(Type))
	{
		UE_LOG(LogTemp, Error, TEXT("UKavunGameInstance::GetLootTable : There is no LootTable of type[%d]"), static_cast<int>(Type));
		return nullptr;
	}
		
	return LootTables[Type];
}
