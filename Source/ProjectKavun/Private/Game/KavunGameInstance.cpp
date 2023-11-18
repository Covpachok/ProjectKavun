#include "Game/KavunGameInstance.h"

#include "Items/ItemBase.h"
#include "Items/LootTableManager.h"

void UKavunGameInstance::Init()
{
	Super::Init();

	LootTableManager = NewObject<ULootTableManager>();
	if ( !IsValid(LootTableManager) )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : LootTableManager has not been created."), __FUNCTIONW__);
	}
	else
	{
		LootTableManager->Init(ItemsDataTable);
	}
}

bool UKavunGameInstance::GetItemData(const FName& ItemName, FItemData& OutItemData) const
{
	if ( !IsValid(ItemsDataTable) )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : ItemsDataTable is invalid"), __FUNCTIONW__);
		return false;
	}

	FItemData* ItemData = ItemsDataTable->FindRow<FItemData>(ItemName, "AKavunGameMode::CheatGiveItem");
	if ( !ItemData )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : Item %s not found."), __FUNCTIONW__, *ItemName.ToString());
		return false;
	}

	OutItemData = *ItemData;
	return true;
}
