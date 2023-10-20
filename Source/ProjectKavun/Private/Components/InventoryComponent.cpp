// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "Characters/KavunCharacter.h"
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

	OwnerCharacter = Cast<AKavunCharacter>(GetOwner());
	if ( IsValid(OwnerCharacter) )
	{
		UE_LOG(LogTemp, Error, TEXT("UInventoryComponent::BeginPlay() : Owner is not a AKavunCharacter"));
	}
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

	Items.Add(Item);
	Item->OnAddedToInventory(OwnerCharacter);
}

void UInventoryComponent::RemoveItem(AItem* Item, bool bDropOnFloor)
{
	// Maybe I don't want to add an ability to remove items :)
}
