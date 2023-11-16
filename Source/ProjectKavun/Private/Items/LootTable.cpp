// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/LootTable.h"

ULootTable::ULootTable()
{
	bWeightCalculated = false;
	OverallWeight     = 0;
}

bool ULootTable::PickRandomItem(FItemData& DataOut) const
{
	if ( LootTable.IsEmpty() )
	{
		UE_LOG(LogTemp, Error, TEXT("ALootTable::PickRandomItem : %s is empty."), *GetName());
		return false;
	}

	const int PickedWeight = FMath::RandRange(1, OverallWeight);

	int CurrentWeight = 0;
	for ( const auto& Entry : LootTable )
	{
		CurrentWeight += Entry.Weight;
		if ( PickedWeight <= CurrentWeight )
		{
			DataOut = *Entry.ItemData;
			return true;
		}
	}

	UE_LOG(LogTemp, Error, TEXT("ALootTable::PickRandomItem : Somehow chosen weight(%d out of %d) is wrong."),
	       CurrentWeight, OverallWeight);
	return false;
}

void ULootTable::AddItem(const FItemData* Data)
{
	if ( !Data )
	{
		UE_LOG(LogTemp, Error, TEXT("ALootTable::AddItem : Passed FItemData is null."));
		return;
	}

	LootTable.Add(FLootTableEntry{static_cast<int>(Data->Quality), Data});
}

void ULootTable::CalculateOverallWeight()
{
	for ( const auto& Entry : LootTable )
	{
		OverallWeight += Entry.Weight;
	}

	if ( OverallWeight <= 0 )
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("ALootTable::CalculateOverallWeight : %s weight is less than or equals 0. Somehow."), *GetName());
	}

	bWeightCalculated = true;
}
