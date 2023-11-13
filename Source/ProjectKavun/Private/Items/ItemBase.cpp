// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemBase.h"

DEFINE_LOG_CATEGORY(LogItem);

UItemBase::UItemBase()
{
}

void UItemBase::OnRemovedFromInventory_Implementation(APlayerCharacter* Player, int ItemCount)
{
}

void UItemBase::OnAddedToInventory_Implementation(APlayerCharacter* Player, int ItemCount)
{
}
