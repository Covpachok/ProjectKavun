#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "ItemPedestal.generated.h"

class UItemBase;
enum class ELootTableType;
class UItemDataAsset;

UCLASS()
class PROJECTKAVUN_API AItemPedestal : public AActor
{
	GENERATED_BODY()

public:
	AItemPedestal();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Item Pedestal")
	UItemDataAsset* TakeItemData();
	
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	               int32                OtherBodyIndex, bool         bFromSweep, const FHitResult&    SweepResult);

private:
	void InitItem();

private:
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UCapsuleComponent> CapsuleCollision;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> ItemMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> PedestalMesh;

	UPROPERTY(EditAnywhere)
	bool bTakeFromLootTable;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bTakeFromLootTable == false", EditConditionHides = true))
	TObjectPtr<UItemDataAsset> ItemData;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bTakeFromLootTable == true", EditConditionHides = true))
	ELootTableType LootTableSource;

	UPROPERTY(VisibleInstanceOnly)
	bool bHasItem;

	TObjectPtr<UItemBase> ItemInstance;
};
