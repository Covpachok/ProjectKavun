// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class APlayerCharacter;
class UItemDataAsset;
class AKavunCharacter;
class UItemBase;

USTRUCT()
struct FInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UItemBase> Item;

	UPROPERTY(VisibleAnywhere)
	int Count;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTKAVUN_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(UItemBase* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItem(FName ItemName, bool bDropOnFloor = false);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<APlayerCharacter> PlayerOwner;

	UPROPERTY(VisibleAnywhere)
	TMap<FName, FInventorySlot> Items;
};
