// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "GameFramework/Actor.h"
#include "LootTable.generated.h"

struct FLootTableEntry
{
	int              Weight;
	const FItemData* ItemData;
};

UCLASS()
class PROJECTKAVUN_API ULootTable : public UObject
{
	GENERATED_BODY()

public:
	ULootTable();

	UFUNCTION(BlueprintCallable, Category = "Loot Table")
	bool PickRandomItem(FItemData& DataOut) const;

	void AddItem(const FItemData* Data);

	void CalculateOverallWeight();

private:
	TArray<FLootTableEntry> LootTable;

	UPROPERTY(VisibleInstanceOnly)
	int OverallWeight;

	bool bWeightCalculated;
};
