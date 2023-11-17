#include "Game/KavunGameInstance.h"

#include "Items/ItemBase.h"
#include "Items/LootTableManager.h"

void UKavunGameInstance::Init()
{
	Super::Init();

	LootTableManager = NewObject<ULootTableManager>();
	if ( !IsValid(LootTableManager) )
	{
		UE_LOG(LogTemp, Error, TEXT("UKavunGameInstance::Init : LootTableManager has not been created."));
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
		UE_LOG(LogTemp, Error, TEXT("AKavunGameInstance::GetItemData : ItemsDataTable is invalid"));
		return false;
	}

	FItemData* ItemData = ItemsDataTable->FindRow<FItemData>(ItemName, "AKavunGameMode::CheatGiveItem");
	if ( !ItemData )
	{
		UE_LOG(LogTemp, Error, TEXT("AKavunGameInstance::GetItemData : Item %s not found."), *ItemName.ToString());
		return false;
	}

	OutItemData = *ItemData;
	return true;
}
