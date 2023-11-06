// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

class UCapsuleComponent;
DECLARE_LOG_CATEGORY_EXTERN(LogItem, Log, All);

class AKavunCharacter;

USTRUCT(BlueprintType, Blueprintable)
struct FItemData : public FTableRowBase
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

	FItemData()
		: Name(TEXT("Unknown")),
		  Id(-1)
	{
	}
};


UCLASS()
class PROJECTKAVUN_API AItemBase : public AActor
{
	GENERATED_BODY()

public:
	AItemBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, Category = "Item|Inventory")
	void OnPickedUp(AKavunCharacter* Player);

	UFUNCTION(BlueprintCallable, Category = "Item|Data")
	int32 GetId() const { return ItemData.Id; }

	UFUNCTION(BlueprintCallable, Category = "Item|Data")
	FItemData GetData() const { return ItemData; }


	UFUNCTION(BlueprintCallable, Category = "Item")
	void Enable();

	UFUNCTION(BlueprintCallable, Category = "Item")
	/** Item still be ticking */
	void Disable();

private:
	UPROPERTY(EditAnywhere, Category = "Data")
	FDataTableRowHandle DataTableRowHandle;

	UPROPERTY(VisibleAnywhere, Category = "Data")
	FItemData ItemData;


	UPROPERTY(EditAnywhere, Category = "Components")
	UCapsuleComponent* CapsuleCollision;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComponent;
};
