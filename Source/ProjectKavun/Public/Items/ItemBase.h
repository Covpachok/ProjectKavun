// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/Optional.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

class UItemBase;
enum class ELootTableType : uint8;

UENUM()
enum class EItemQuality
{
	S = 1,
	A = 2,
	B = 3,
	C = 4,
	D = 5
};

USTRUCT(Blueprintable, BlueprintType)
struct PROJECTKAVUN_API FItemData : public FTableRowBase
{
	GENERATED_BODY()

	FItemData()
		: Quality(EItemQuality::B)
	{
	};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	/* Item name that will be used as an ID */
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	/* Item name that will appear in UI and can be localized */
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	/* This cool wooden chair was once a piece of wood, you know */
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	/* Item icon for UI */
	TObjectPtr<UTexture> Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	/* Item 3d model */
	TObjectPtr<UStaticMesh> StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	/* Quality affects rarity */
	EItemQuality Quality;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	/* Where this item can be found */
	TSet<ELootTableType> LootTables;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	/* Item class with actual functionality */
	TSubclassOf<UItemBase> ItemClass;
};

class APlayerCharacter;
DECLARE_LOG_CATEGORY_EXTERN(LogItem, Log, All);

UCLASS(Abstract, Blueprintable, BlueprintType)
class PROJECTKAVUN_API UItemBase : public UObject
{
	GENERATED_BODY()

public:
	UItemBase();

	UFUNCTION(BlueprintCallable, Category = "Item")
	void InitData();

	UFUNCTION(BlueprintNativeEvent, Category = "Item")
	/* Inside inventory there is only one instance of an Item, yet player can collect a lot of them.
	 * So OnAddedToInventory shouldn't duplicate some of the buffs of an Item. */
	void OnAddedToInventory(APlayerCharacter* Player, int ItemCount);

	UFUNCTION(BlueprintNativeEvent, Category = "Item")
	/* Inside inventory there is only one instance of an Item, yet player can collect a lot of them.
	 * So OnRemovedFromInventory isn't always mean that this Item will be removed completely. */
	void OnRemovedFromInventory(APlayerCharacter* Player, int ItemCount);

	UFUNCTION(BlueprintCallable, Category = "Item")
	const FItemData& GetData() const { return ItemData; }

private:
	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle ItemDataRow;

	FItemData ItemData;
};
