

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LootTableManager.generated.h"

class ULootTable;

UENUM()
enum class ELootTableType : uint8
{
	Generic,
	Treasure,
	// ...
};

ENUM_RANGE_BY_FIRST_AND_LAST(ELootTableType, ELootTableType::Generic, ELootTableType::Treasure);

UCLASS()
class PROJECTKAVUN_API ULootTableManager : public UObject
{
	GENERATED_BODY()

public:
	ULootTableManager();

	void Init(const UDataTable* ItemsTable);

	UFUNCTION(BlueprintCallable, Category = "Loot Table")
	ULootTable *GetLootTable(ELootTableType Type) const;

private:
	TMap<ELootTableType, TObjectPtr<ULootTable>> LootTables;
};
