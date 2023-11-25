#pragma once

#include "CoreMinimal.h"
#include "Actors/InteractableActorBase.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Chest.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChestOpenedDelegate);

struct FItemData;
class APickupBase;
class AItemPedestal;

USTRUCT()
struct FPickupsDropData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<APickupBase> PickupClass;
	
	UPROPERTY(EditAnywhere, meta = (UiMin = 0.01, ClampMin = 0.01))
	float Weight;
};

UCLASS()
class AChest : public AInteractableActorBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Chest")
	FOnChestOpenedDelegate OnChestOpened;

	AChest();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void OnInteracted_Implementation(APlayerCharacter* Player) override;

private:
	void CalculatePickupsOverallWeight();

	void SpawnItemPedestal();
	void DropPickups();

	UFUNCTION()
	void OnItemPedestalUsed(const FItemData &ItemData);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> TrunkMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> CoverMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UBoxComponent> Collision;

private:
	UPROPERTY(EditAnywhere)
	bool bClosedOnKey;

	UPROPERTY(VisibleInstanceOnly)
	bool bOpened;

	
	/*********************
	 * ITEM DROP SECTION
	 *********************/
	
	UPROPERTY(EditAnywhere, Category = "Drop Settings")
	bool bCanSpawnItem;

	UPROPERTY(EditAnywhere, Category = "Drop Settings|Item",
		meta = (EditCondition = "bCanSpawnItem", EditConditionHides = true,
			UIMin = 0, ClampMin = 0, UIMax = 1, ClampMax = 1))
	float ItemProbability;

	UPROPERTY(EditAnywhere, Category = "Drop Settings|Item",
		meta = (EditCondition = "bCanSpawnItem", EditConditionHides = true))
	TSubclassOf<AItemPedestal> ItemPedestalClass;

	TObjectPtr<AItemPedestal> ItemPedestalInstance;
	
	/*********************
	 * PICKUPS DROP SECTION
	 *********************/

	UPROPERTY(EditAnywhere, Category = "Drop Settings|Pickups")
	TArray<FPickupsDropData> PickupsDropData;

	UPROPERTY(EditAnywhere, Category = "Drop Settings|Pickups", meta = (UIMin = 1, ClampMin = 1))
	int MinPickupsDrop;
	
	UPROPERTY(EditAnywhere, Category = "Drop Settings|Pickups", meta = (UIMin = 1, ClampMin = 1))
	int MaxPickupsDrop;

	UPROPERTY(VisibleInstanceOnly, Category = "Drop Settings|Pickups")
	float PickupsOverallWeight;
};