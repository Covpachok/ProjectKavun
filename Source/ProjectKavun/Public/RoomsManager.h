// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rooms/RoomBase.h"
#include "LevelGenerator.h"
#include "RoomsManager.generated.h"

struct FLevelRoom;

USTRUCT()
struct FRoomWallMeshes
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMesh *DefaultWall;
	
	UPROPERTY(EditAnywhere)
	UStaticMesh *DefaultDoorway;
	
	UPROPERTY(EditAnywhere)
	UStaticMesh *ShortWall;
	
	UPROPERTY(EditAnywhere)
	UStaticMesh *ShortDoorway;
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

	void OnLevelGenerationCompleted(const FLevelMap& LevelMap, const TArray<FIntPoint>& RoomLocations);

private:
	ARoomBase* SpawnRoom(const FLevelRoom& LevelRoom,
	               const FVector&   WorldLocation);
	void ChangeRoomWalls(ARoomBase *RoomActor, const FLevelMap& LevelMap, const FLevelRoom& LevelRoom, const FIntPoint& LevelLocation);

private:
	UPROPERTY(EditAnywhere)
	FVector RoomsLocationDelta;

	UPROPERTY(EditAnywhere)
	TMap<ERoomShape, TSubclassOf<ARoomBase>> RoomClasses;

	UPROPERTY(EditAnywhere)
	TMap<FIntPoint, FRoomWallMeshes> WallMeshes;
};
