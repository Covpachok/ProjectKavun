// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rooms/RoomBase.h"
#include "LevelGenerator.generated.h"

class ARoomsManager;
class FLevelMap;

UCLASS()
class PROJECTKAVUN_API ALevelGenerator : public AActor
{
	GENERATED_BODY()

public:
	ALevelGenerator();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void GenerateLevel();

private:
	void GenerateRoom(const FIntVector2& Location, ERoomShape RoomShape, bool bCanGiveUp = true);

	void GenerateNeighborFor(const FIntVector2& ForLocation);

	bool FindUnoccupiedNeighbor(const FIntVector2& ForLocation, FIntVector2& ReturnNeighborLocation) const;

	bool CanPlaceRoom(const FIntVector2& Location, ERoomShape Shape, FIntVector2& Correction);
	bool CanBePlacedAt(const FIntVector2& Location, const FRoomShapeDetails& ShapeDetails);
	bool CanBePlacedNear(const FIntVector2& PlaceLocation, const FIntVector2& NeighborLocation) const;

private:
	UPROPERTY(EditAnywhere)
	bool bGenerateAtStart;

	UPROPERTY(EditAnywhere)
	FVector RoomDelta;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 11, UIMin = 11))
	int LevelWidth;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 11, UIMin = 11))
	int LevelHeight;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 1, UIMin = 1))
	int MinRoomsCount;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 1, UIMin = 1))
	int MaxRoomsCount;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0, UIMin = 0))
	int MaxBigRooms;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0, UIMin = 0))
	int MaxNarrowRooms;

	UPROPERTY(EditAnywhere)
	ARoomsManager* RoomsManager;

	UPROPERTY(EditAnywhere)
	int DebugUpperShape;

	TSharedPtr<FLevelMap> LevelMap;

	TArray<FIntVector2> GeneratedRoomLocations;

	int GeneratedRoomsCount;
	int TargetRoomsCount;

	int BigRoomsPlaced;
	int NarrowRoomsPlaced;
};
