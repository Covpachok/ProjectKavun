// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/LootTable.h"

ULootTable::ULootTable()
{
	bWeightCalculated = false;
	OverallWeight     = 0;
}

const UItemDataAsset* ULootTable::PickRandomItem()
{
	if ( LootTable.IsEmpty() )
	{
		UE_LOG(LogTemp, Error, TEXT("ALootTable::PickRandomItem : %s is empty."), *GetName());
		return nullptr;
	}

	if ( !bWeightCalculated )
	{
		CalculateOverallWeight();
	}

	const int PickedWeight = FMath::RandRange(1, OverallWeight);

	int CurrentWeight = 0;
	for ( const auto& Entry : LootTable )
	{
		if ( PickedWeight < CurrentWeight )
		{
			return Entry.ItemData;
		}
		CurrentWeight += Entry.Weight;
	}

	UE_LOG(LogTemp, Warning, TEXT("ALootTable::PickRandomItem : Somehow chosen weight(%d out of %d) is wrong."),
	       CurrentWeight, OverallWeight);
	return LootTable[0].ItemData;
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
