// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "Characters/KavunCharacter.h"
#include "Items/ItemBase.h"
#include "Items/ItemDataAsset.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AKavunCharacter>(GetOwner());
	if ( IsValid(OwnerCharacter) )
	{
		UE_LOG(LogTemp, Error, TEXT("UInventoryComponent::BeginPlay : Owner is not a AKavunCharacter"));
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
	if ( Items.Contains(ItemName) )
	{
		++Items[ItemName].Count;
		// Maybe should destroy this item because it isn't needed anymore.
	}
	else
	{
		Items.Add(ItemName, FInventorySlot{Item, 1});
	}
}

void UInventoryComponent::RemoveItem(FName ItemName, bool bDropOnFloor)
{
	if(!Items.Contains(ItemName))
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::RemoveItem : Item with Name[%s]"), *ItemName.GetPlainNameString());
		return;
	}

	FInventorySlot &Slot = Items[ItemName];
	if(Slot.Count > 1)
	{
		--Slot.Count;
		// Do other stuff
	}
	else
	{
		Items.Remove(ItemName);
		// Do other stuff
	}
}
