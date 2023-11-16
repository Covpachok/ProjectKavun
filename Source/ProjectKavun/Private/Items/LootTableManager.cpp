#include "Items/LootTableManager.h"

#include "Items/ItemBase.h"
#include "Items/LootTable.h"

ULootTableManager::ULootTableManager()
{
	for ( ELootTableType Type : TEnumRange<ELootTableType>() )
	{
		ULootTable* LootTable = NewObject<ULootTable>();
		if ( !IsValid(LootTable) )
		{
			UE_LOG(LogTemp, Error,
			       TEXT(
				       "ULootTableManager::ULootTableManager : Created LootTable is invalid. How is that even possible?"
			       ));
			return;
		}
		LootTables.Add(Type, LootTable);
	}
}

void ULootTableManager::Init(const UDataTable* ItemsTable)
{
	if ( !IsValid(ItemsTable) )
	{
		UE_LOG(LogTemp, Error, TEXT("ULootTableManager::Init : ItemsTable is invalid."));
		return;
	}

	for ( ELootTableType Type : TEnumRange<ELootTableType>() )
	{
		if ( !LootTables.Contains(Type) )
		{
			UE_LOG(LogTemp, Error, TEXT("ULootTableManager::Init : LootTable of type(%d) not found."),
			       static_cast<uint8>(Type));
			return;
		}
	}

	TArray<FItemData*> ItemDataRows;
	ItemsTable->GetAllRows<FItemData>("ULootTableManager::Init", ItemDataRows);

	if ( ItemDataRows.IsEmpty() )
	{
		UE_LOG(LogTemp, Error, TEXT("ULootTableManager::Init : ItemDataRows is empty."));
		return;
	}

	for ( const auto& ItemData : ItemDataRows )
	{
		if ( !ItemData )
		{
			UE_LOG(LogTemp, Error, TEXT("ULootTableManager::Init : ItemData is null."));
			return;
		}

		for ( const auto& LootTableType : ItemData->LootTables )
		{
			LootTables[LootTableType]->AddItem(ItemData);
		}
	}

	for ( auto& LootTable : LootTables )
	{
		LootTable.Value->CalculateOverallWeight();
	}
}

ULootTable* ULootTableManager::GetLootTable(ELootTableType Type) const
{
	const TObjectPtr<ULootTable>* LootTablePtr = LootTables.Find(Type);
	if ( !LootTablePtr )
	{
		UE_LOG(LogTemp, Error, TEXT("ULootTableManager::GetLootTable : LootTable of type(%d) not found."),
		       static_cast<uint8>(Type));
		return nullptr;
	}
	return *LootTablePtr;
}
