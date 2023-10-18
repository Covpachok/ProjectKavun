// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include "Items/Item.h"

UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


void UInventoryComponent::TickComponent(float                        DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::AddItem(AItem* Item)
{
	if ( !IsValid(Item) )
	{
		return;
	}

	int32 Id = Item->GetId();
	if ( Items.Contains(Id) )
	{
		// Items[Id].AddCount();
		Items.Add(Item->GetId(), Item);
	}
}

void UInventoryComponent::RemoveItem(AItem* Item, bool bDropOnFloor)
{
}
