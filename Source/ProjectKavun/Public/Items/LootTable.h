// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "GameFramework/Actor.h"
#include "LootTable.generated.h"

USTRUCT()
struct FLootTableEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int Weight;

	UPROPERTY(EditAnywhere)
	FItemData ItemData;
};

UCLASS()
class PROJECTKAVUN_API ALootTable : public AActor
{
	GENERATED_BODY()

public:
	ALootTable();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Loot Table")
	const FItemData &PickRandomItem() const;

private:
	void CalculateOverallWeight();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Entries")
	TArray<FLootTableEntry> LootTableEntries;

	UPROPERTY(VisibleInstanceOnly, Category = "Entries")
	int OverallWeight;
};
