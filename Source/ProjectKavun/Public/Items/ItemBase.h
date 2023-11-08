// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogItem, Log, All);

class UItemDataAsset;

UCLASS(Abstract)
class PROJECTKAVUN_API UItemBase : public UObject
{
	GENERATED_BODY()

public:
	UItemBase();

	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void Use() {};

	UFUNCTION(BlueprintGetter)
	const UItemDataAsset* GetData() const { return ItemData; }

private:
	UPROPERTY(EditDefaultsOnly, BlueprintGetter = "GetData", Category = "Data")
	TObjectPtr<UItemDataAsset> ItemData;
};
