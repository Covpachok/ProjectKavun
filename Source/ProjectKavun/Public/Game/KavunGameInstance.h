#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "KavunGameInstance.generated.h"

class ULootTableManager;
class UActorPoolComponent;

UCLASS()
class PROJECTKAVUN_API UKavunGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	UFUNCTION(BlueprintGetter, Category = "Loot Table")
	ULootTableManager* GetLootTableManager() const { return LootTableManager; }

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> ItemsDataTable;

	UPROPERTY(BlueprintGetter = "GetLootTableManager")
	TObjectPtr<ULootTableManager> LootTableManager;
};
