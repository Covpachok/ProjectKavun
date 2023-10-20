// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"

DEFINE_LOG_CATEGORY(LogItem);

AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	FItemData* Data = DataTableRowHandle.GetRow<FItemData>(DataTableRowHandle.RowName.ToString());
	if ( Data == nullptr )
	{
		UE_LOG(LogItem, Error, TEXT("AItem::BeginPlay : GetRow<FItemData> is unsuccessful, ItemData is null."));
	}
	else
	{
		ItemData = *Data;
	}
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItem::OnAddedToInventory_Implementation(AKavunCharacter* InventoryOwner)
{
}

void AItem::OnRemovedFromInventory_Implementation(AKavunCharacter* InventoryOwner, bool bDropOnFloor)
{
}
