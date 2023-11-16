#include "Game/KavunGameInstance.h"

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
