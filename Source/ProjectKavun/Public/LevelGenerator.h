// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rooms/LevelRoomBase.h"
#include "LevelGenerator.generated.h"


class ARoomsManager;

UCLASS()
class PROJECTKAVUN_API ALevelGenerator : public AActor
{
	GENERATED_BODY()

	struct FLevelRoom
	{
		bool Occupied = false;

		int Id = 0;
	};

public:
	ALevelGenerator();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void GenerateLevel();

private:
	void GenerateRoom(const FIntVector2& Location, ERoomShape RoomShape, bool bCanGiveUp = true);

	void GenerateNeighborFor(const FIntVector2& OriginLocation, ERoomShape OriginShape);

	bool ChooseUnoccupiedNeighbor(const FIntVector2& ForLocation, FIntVector2& NeighborLocation) const;

	int CountNeighbours(const FIntVector2& Location);

	bool CanPlaceRoomShape(const FIntVector2& Location, ERoomShape Shape, FIntVector2& PossibleCorrection);
	bool RoomShapeInteralCheck(const FIntVector2& Location, FRoomShapeDetails ShapeDetails);

	void PrintLevel();

	bool IsInBounds(const FIntVector2& Location) const
	{
		return Location.X < MaxLevelWidth && Location.X >= 0 && Location.Y < MaxLevelHeight && Location.Y >= 0;
	}

	bool IsOccupied(const FIntVector2& Location) const
	{
		return LevelMap[Location.Y][Location.X].Occupied;
	}

	bool IsOccupiedSafe(const FIntVector2& Location) const
	{
		return IsInBounds(Location) ? LevelMap[Location.Y][Location.X].Occupied : false;
	}

public:
	UPROPERTY(EditAnywhere)
	bool bGenerateAtStart;

	UPROPERTY(EditAnywhere)
	FVector RoomDelta;

	UPROPERTY(EditAnywhere)
	int MaxLevelWidth;

	UPROPERTY(EditAnywhere)
	int MaxLevelHeight;

	UPROPERTY(EditAnywhere)
	int MinRoomsAmount;

	UPROPERTY(EditAnywhere)
	int MaxRoomsAmount;

	UPROPERTY(EditAnywhere)
	int MaxBigRooms;

	UPROPERTY(EditAnywhere)
	ARoomsManager* RoomsManager;

	TArray<TArray<FLevelRoom>> LevelMap;

	FIntVector2 CentralRoomLocation;

	TArray<FIntVector2> GeneratedRoomsLocations;
	TArray<ERoomShape>  GeneratedRoomsShapes;

	int GeneratedRoomsAmount;
	int ChosenRoomsAmount;

	int BigRoomsPlaced;
};
