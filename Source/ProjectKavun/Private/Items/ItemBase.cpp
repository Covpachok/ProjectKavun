// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemBase.h"

#include "Components/CapsuleComponent.h"

DEFINE_LOG_CATEGORY(LogItem);

AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	SetRootComponent(CapsuleCollision);
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);
}

void AItemBase::BeginPlay()
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

void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItemBase::OnPickedUp_Implementation(AKavunCharacter* Player)
{
}

void AItemBase::Enable()
{
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
}

void AItemBase::Disable()
{
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
}