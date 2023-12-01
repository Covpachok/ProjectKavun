// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rooms/RoomBase.h"
#include "Level/LevelGenerator.h"
#include "RoomsManager.generated.h"

class ADoor;
struct FLevelRoom;

DECLARE_LOG_CATEGORY_EXTERN(RoomsManagerLog, Log, All);

USTRUCT()
struct FRoomWallMeshes
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMesh* DefaultWall;

	UPROPERTY(EditAnywhere)
	UStaticMesh* DefaultDoorway;

	UPROPERTY(EditAnywhere)
	UStaticMesh* ShortWall;

	UPROPERTY(EditAnywhere)
	UStaticMesh* ShortDoorway;
};

struct FRoomConnection
{
	FIntPoint Point1;
	FIntPoint Point2;
};

UCLASS()
class PROJECTKAVUN_API ARoomsManager : public AActor
{
	GENERATED_BODY()

public:
	ARoomsManager();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void OnLevelGenerationCompleted(FLevelMap& LevelMap, const TArray<FIntPoint>& RoomLocations);
	
private:
	ARoomBase* SpawnRoom(const FLevelRoom& LevelRoom,
	                     const FVector&    WorldLocation);

	void ConstructRoom(ARoomBase&       RoomActor, const FLevelMap& LevelMap, const FLevelRoom& LevelRoom,
	                   const FIntPoint& LevelLocation);

	void SpawnDecoration(ARoomBase& RoomActor, const FLevelRoom& LevelRoom);

	void ChangeRoomWalls(TArray<UWallComponent*>& Walls, const FLevelMap& LevelMap, const FLevelRoom& LevelRoom,
	                     const FIntPoint&         LevelLocation);

	void PlaceDoors(const FLevelMap& LevelMap);

	FVector MapToWorldRoomLocation(const FIntPoint& RoomLocation) const;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> RoomDecorationsDataTable;

	UPROPERTY(EditAnywhere)
	FVector RoomsLocationDelta;

	UPROPERTY(EditAnywhere)
	TMap<ERoomShape, TSubclassOf<ARoomBase>> RoomClassesByShape;

	UPROPERTY(EditAnywhere)
	TMap<ERoomType, TSubclassOf<ARoomBase>> RoomClassesByType;

	UPROPERTY(EditAnywhere)
	TMap<FIntPoint, FRoomWallMeshes> WallMeshes;

	UPROPERTY(EditAnywhere)
	TMap<ERoomType, TObjectPtr<UStaticMesh>> DoorMeshes;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ADoor> DoorClass;

	FIntPoint CentralRoomLocation;

	// TMap<ERoomShape, UDecorationTable> DecorationsTable;
};
