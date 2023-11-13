// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

class APlayerCharacter;
DECLARE_LOG_CATEGORY_EXTERN(LogItem, Log, All);

class UItemDataAsset;

UCLASS(Abstract, Blueprintable, BlueprintType)
/* Item functionality object, other stuff is inside DataAsset */
class PROJECTKAVUN_API UItemBase : public UObject
{
	GENERATED_BODY()

public:
	UItemBase();

	UFUNCTION(BlueprintNativeEvent, Category = "Item")
	/* Inside inventory there is only one instance of an Item, yet player can collect a lot of them.
	 * So OnAddedToInventory shouldn't duplicate some of the buffs of an Item. */
	void OnAddedToInventory(APlayerCharacter* Player, int ItemCount);

	UFUNCTION(BlueprintNativeEvent, Category = "Item")
	/* Inside inventory there is only one instance of an Item, yet player can collect a lot of them.
	 * So OnRemovedFromInventory isn't always mean that this Item will be removed completely. */
	void OnRemovedFromInventory(APlayerCharacter* Player, int ItemCount);

	UFUNCTION(BlueprintGetter, Category = "Item")
	const UItemDataAsset* GetData() const { return ItemData; }

	void SetData(UItemDataAsset* Data) { ItemData = Data; }

private:
	UPROPERTY(EditDefaultsOnly, BlueprintGetter = "GetData", Category = "Data")
	TObjectPtr<UItemDataAsset> ItemData;
};
