// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"

#include "Components/CapsuleComponent.h"

DEFINE_LOG_CATEGORY(LogItem);

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	SetRootComponent(CapsuleCollision);
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	const FItemData* Data = DataTableRowHandle.GetRow<FItemData>(DataTableRowHandle.RowName.ToString());
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

void AItem::Enable()
{
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
}

void AItem::Disable()
{
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
}

void AItem::OnAddedToInventory_Implementation(AKavunCharacter* InventoryOwner)
{
}

void AItem::OnRemovedFromInventory_Implementation(AKavunCharacter* InventoryOwner, bool bDropOnFloor)
{
}
