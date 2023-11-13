#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "KavunGameInstance.generated.h"

class UActorPoolComponent;
enum class ELootTableType;
class ULootTable;

UCLASS()
class PROJECTKAVUN_API UKavunGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Gameplay Globals")
	ULootTable* GetLootTable(ELootTableType Type);

private:
	UPROPERTY(EditAnywhere, Category = "Gameplay Globals")
	TMap<ELootTableType, TObjectPtr<ULootTable>> LootTables;

	// UPROPERTY(EditAnywhere, Category = "Gameplay Globals")
	// TMap<UClass, TObjectPtr<UActorPoolComponent>> ActorPool;
};
