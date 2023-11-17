// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemBase.h"
#include "Items/ItemDataAsset.h"

DEFINE_LOG_CATEGORY(LogItem);

UItemBase::UItemBase()
{
}

void UItemBase::InitData()
{
	FItemData *ItemDataPtr = ItemDataRow.GetRow<FItemData>("UItemBase::InitData");
	if(!ItemDataPtr)
	{
		UE_LOG(LogTemp, Error, TEXT("UItemBase::InitData : ItemData not found in data table."));
		return;
	}
	ItemData = *ItemDataPtr;
}

void UItemBase::OnRemovedFromInventory_Implementation(APlayerCharacter* Player, int ItemCount)
{
}

void UItemBase::OnAddedToInventory_Implementation(APlayerCharacter* Player, int ItemCount)
{
}
