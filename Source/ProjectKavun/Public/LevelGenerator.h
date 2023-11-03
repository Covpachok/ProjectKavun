// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelGenerator.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LevelGeneratorLog, Log, All);

enum class ERoomShape : uint8;
enum class ERoomType : uint8;

struct FRoomShapeDetails;

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
	void GenerateRoom(const FIntPoint& Location, ERoomShape RoomShape, bool bCanGiveUp = true);
	void GenerateSpecialRooms();

	void GenerateNeighborFor(const FIntPoint& ForLocation);

	bool FindUnoccupiedNeighbor(const FIntPoint& ForLocation, FIntPoint& ReturnNeighborLocation) const;

	bool CanPlaceRoom(const FIntPoint& Location, ERoomShape Shape, FIntPoint& Correction);
	bool CanBePlacedAt(const FIntPoint& Location, const FRoomShapeDetails& ShapeDetails);
	bool CanBePlacedNear(const FIntPoint& PlaceLocation, const FIntPoint& NeighborLocation) const;

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

	TArray<FIntPoint> GeneratedRoomLocations;

	int GeneratedRoomsCount;
	int TargetRoomsCount;

	int BigRoomsPlaced;
	int NarrowRoomsPlaced;
};
