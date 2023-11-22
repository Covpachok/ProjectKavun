#pragma once

#include "CoreMinimal.h"
#include "Characters/KavunCharacterBase.h"
#include "GameFramework/Actor.h"
#include "RoomDecorationBase.generated.h"

class ARoomDecorationBase;
enum class ERoomType : uint8;
enum class ERoomShape : uint8;

class UEnemySpawnerComponent;
class UActorSpawnerComponent;

USTRUCT()
struct FRoomDecorationInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ERoomShape SuitableRoomShape;

	UPROPERTY(EditAnywhere)
	ERoomType SuitableRoomType;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ARoomDecorationBase> DecorationClass;
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

private:
	TArray<TObjectPtr<UActorSpawnerComponent>> ActorSpawners;

	TArray<TObjectPtr<UEnemySpawnerComponent>> EnemySpawners;

	int EnemiesCount;
	int EnemyDeathCount;
};
