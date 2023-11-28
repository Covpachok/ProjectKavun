#pragma once

#include "CoreMinimal.h"
#include "Characters/KavunCharacterBase.h"
#include "GameFramework/Actor.h"
#include "Rooms/RoomDetails.h"
#include "RoomDecorationBase.generated.h"

class ARoomDecorationBase;

class UEnemySpawnerComponent;
class UActorSpawnerComponent;

USTRUCT()
struct FRoomDecorationInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ERoomShape SuitableRoomShape = ERoomShape::Square;

	UPROPERTY(EditAnywhere)
	ERoomType SuitableRoomType = ERoomType::Default;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ARoomDecorationBase> DecorationClass = nullptr;
};

UCLASS(Abstract)
class PROJECTKAVUN_API ARoomDecorationBase : public AActor
{
	GENERATED_BODY()

public:
	ARoomDecorationBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Room|Decoration")
	void ActivateSpawners();

	UFUNCTION(BlueprintCallable, Category = "Room|Decoration")
	void DeactivateSpawners();

private:
	UFUNCTION()
	void CountEnemyDeaths(AKavunCharacterBase* Character);

protected:
	TArray<TObjectPtr<UActorSpawnerComponent>> ActorSpawners;

	TArray<TObjectPtr<UEnemySpawnerComponent>> EnemySpawners;

	int EnemiesCount;
	int EnemyDeathCount;
};
