// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "Characters/PlayerCharacter.h"
#include "Items/ItemBase.h"
#include "Items/ItemDataAsset.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerOwner = Cast<APlayerCharacter>(GetOwner());
	if ( !IsValid(PlayerOwner) )
	{
		UE_LOG(LogTemp, Error, TEXT("UInventoryComponent::BeginPlay : Owner is not a APlayerCharacter"));
	}
}

void UInventoryComponent::AddItem(UItemBase* Item)
{
	if ( !IsValid(Item) )
	{
		UE_LOG(LogTemp, Error, TEXT("UInventoryComponent::AddItem : Item is invalid"));
		return;
	}

	const FName ItemName = Item->GetData()->NameID;
	int         ItemCount;

	if ( !Items.IsEmpty() && Items.Contains(ItemName) )
	{
		ItemCount = ++Items[ItemName].Count;
	}
	else
	{
		Items.Add(ItemName, FInventorySlot{Item, 1});
		ItemCount = 1;
	}

	Item->OnAddedToInventory(PlayerOwner, ItemCount);
}

void UInventoryComponent::RemoveItem(FName ItemName, bool bDropOnFloor)
{
	if ( !Items.Contains(ItemName) )
	{
		UE_LOG(LogTemp, Error, TEXT("UInventoryComponent::RemoveItem : Item with Name[%s] isn't found"),
		       *ItemName.GetPlainNameString());
		return;
	}

	FInventorySlot& Slot         = Items[ItemName];
	UItemBase*      ItemToRemove = Slot.Item;

	--Slot.Count;
	if ( Slot.Count == 0 )
	{
		Items.Remove(ItemName);
	}

	if ( !IsValid(ItemToRemove) )
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("UInventoryComponent::RemoveItem : Item in Inventory is invalid for some reason."
			       " OnRemovedFromInventory won't be called, yet item will be removed."));
	}
	else
	{
		ItemToRemove->OnRemovedFromInventory(PlayerOwner, Slot.Count);
	}

	if ( bDropOnFloor )
	{
		// Create new ItemPedestal and put item on it.
	}
}
