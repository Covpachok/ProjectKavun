// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKAVUN_API UItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// UFUNCTION(BlueprintCallable, Category = "Item Data")
	// UItemBase* CreateItem();
	//
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// /* Item name that will be used as an ID */
	// FName Name;
	//
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// /* Item name that will appear in UI and can be localized */
	// FText DisplayName;
	//
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// /* This cool wooden chair was once a piece of wood, you know */
	// FText Description;
	//
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// /* Item icon for UI */
	// TObjectPtr<UTexture> Icon;
	//
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// /* Item 3d model */
	// TObjectPtr<UStaticMesh> StaticMesh;
	//
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// /* Quality affects rarity */
	// EItemQuality Quality;
	//
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// /* Where this item can be found */
	// TSet<ELootTableType> LootTables;
	//
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// /* Item class with actual functionality */
	// TSubclassOf<UItemBase> ItemClass;
};
//
//
// USTRUCT(Blueprintable, BlueprintType)
// struct PROJECTKAVUN_API FItemData : public FTableRowBase
// {
// 	GENERATED_BODY()
// 	
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
// 	/* Item name that will be used as an ID */
// 	FName Name;
//
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
// 	/* Item name that will appear in UI and can be localized */
// 	FText DisplayName;
//
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
// 	/* This cool wooden chair was once a piece of wood, you know */
// 	FText Description;
//
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
// 	/* Item icon for UI */
// 	TObjectPtr<UTexture> Icon;
//
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
// 	/* Item 3d model */
// 	TObjectPtr<UStaticMesh> StaticMesh;
//
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
// 	/* Quality affects rarity */
// 	EItemQuality Quality;
//
// 	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
// 	// /* Where this item can be found */
// 	// TSet<ELootTableType> LootTables;
//
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
// 	/* Item class with actual functionality */
// 	TSubclassOf<UItemBase> ItemClass;
// };
