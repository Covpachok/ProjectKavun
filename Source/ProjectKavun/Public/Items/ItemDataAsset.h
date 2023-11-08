// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataAsset.generated.h"

class UItemBase;
/**
 * 
 */
UCLASS()
class PROJECTKAVUN_API UItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	/* Item name that will be used as an ID */
	FName NameID;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	/* Item name that will appear in UI and can be localized */
	FText Name;
	
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
	/* Item class with actual functionality */
	TSubclassOf<UItemBase> Item;
};
