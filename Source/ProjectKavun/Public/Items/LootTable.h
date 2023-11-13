// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "GameFramework/Actor.h"
#include "Misc/Optional.h"
#include "LootTable.generated.h"

UENUM()
// Used in GameInstance
enum class ELootTableType
{
	Generic,
	Treasure,
	// ...
};

USTRUCT()
struct FLootTableEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta=(UiMin=1, ClampMin=1))
	/* Affects the probability of randomly selecting this item.
	 * The higher the weight the higher the probability. */
	int Weight;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UItemDataAsset> ItemData;
};

UCLASS()
class PROJECTKAVUN_API ULootTable : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	ULootTable();

	UFUNCTION(BlueprintCallable, Category = "Loot Table")
	UItemDataAsset* PickRandomItem();

	void CalculateOverallWeight();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Entries")
	TArray<FLootTableEntry> LootTable;

	UPROPERTY(VisibleInstanceOnly, Category = "Entries")
	int OverallWeight;

	bool bWeightCalculated;
};
