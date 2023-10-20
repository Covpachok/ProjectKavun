// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class AKavunCharacter;
class AItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTKAVUN_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(AItem *Item);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItem(AItem *Item, bool bDropOnFloor = false);


private:
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	AKavunCharacter *OwnerCharacter;
	
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<AItem *> Items;
};
