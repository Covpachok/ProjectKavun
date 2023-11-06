// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/LootTable.h"

ALootTable::ALootTable()
{
	PrimaryActorTick.bCanEverTick = false;

	OverallWeight = 0;
}

void ALootTable::BeginPlay()
{
	Super::BeginPlay();

	CalculateOverallWeight();
}

const FItemData& ALootTable::PickRandomItem() const
{
	const int PickedWeight = FMath::RandRange(1, OverallWeight);

	int CurrentWeight = 0;
	for(const auto &Entry: LootTableEntries)
	{
		if(PickedWeight < CurrentWeight)
		{
			return Entry.ItemData;
		}
		CurrentWeight += Entry.Weight;
	}
}

void ALootTable::CalculateOverallWeight()
{
	for(const auto &Entry: LootTableEntries)
	{
		OverallWeight += Entry.Weight;
	}
}

