// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogItem, Log, All);

class AKavunCharacter;

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Id;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTexture> Image;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UClass> Class;
};


UCLASS()
class PROJECTKAVUN_API AItem : public AActor
{
	GENERATED_BODY()

public:
	AItem();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/** --- INVENTORY STUFF --- */
	
	UFUNCTION(BlueprintNativeEvent, Category = "Item")
	void OnAddedToInventory(AKavunCharacter* InventoryOwner);

	UFUNCTION(BlueprintNativeEvent, Category = "Item")
	void OnRemovedFromInventory(AKavunCharacter* InventoryOwner, bool bDropOnFloor);

	
	/** --- GETTERS --- */
	
	UFUNCTION(BlueprintCallable, Category = "Item")
	int32 GetId() const { return ItemData.Id; }

	UFUNCTION(BlueprintCallable, Category = "Item")
	FItemData GetData() const { return ItemData; }

private:
	UPROPERTY(EditAnywhere, Category = "Data")
	FDataTableRowHandle DataTableRowHandle;

	UPROPERTY(VisibleAnywhere, Category = "Data")
	FItemData ItemData;

	UPROPERTY(VisibleAnywhere, Category = "Data")
	int32 InInventoryItemCount;
};
