#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Actors/InteractableActorBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "ItemPedestal.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemPickedUpDelegate, const FItemData&, ItemData);

struct FItemData;
class UItemBase;
enum class ELootTableType : uint8;

UCLASS()
class PROJECTKAVUN_API AItemPedestal : public AInteractableActorBase
{
	GENERATED_BODY()

public:
	FOnItemPickedUpDelegate OnItemPickedUp;
	
	AItemPedestal();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Item Pedestal")
	const FItemData& GetItemData() { return ItemData; }

	virtual void OnInteracted_Implementation(APlayerCharacter* Player) override;

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
	FDataTableRowHandle ItemDataRow;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bTakeFromLootTable == true", EditConditionHides = true))
	ELootTableType LootTableSource;

	UPROPERTY(VisibleInstanceOnly)
	bool bHasItem;

	FItemData ItemData;

	UPROPERTY()
	TObjectPtr<UItemBase> ItemInstance;
};
