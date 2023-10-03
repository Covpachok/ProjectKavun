// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelGenerator.generated.h"

struct FLevelRoom
{
	bool Occupied = false;

	bool Up    = false;
	bool Down  = false;
	bool Left  = false;
	bool Right = false;

	int Id = 0;
};

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
	void GenerateRoom(const FIntVector2& Location);
	void SpawnRoom(const FIntVector2& Location);

	bool ChooseUnoccupiedNeighbor(const FIntVector2& ForLocation, FIntVector2& NeighborLocation) const;

	int CountNeighbours(const FIntVector2& Location);

	void PrintLevel();
	
	bool IsInBounds(const FIntVector2& Location) const
	{
		return Location.X < MaxLevelWidth && Location.X >= 0 && Location.Y < MaxLevelHeight && Location.Y >= 0;
	}

public:
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
	// Replace AActor with a Room class later
	TSubclassOf<AActor> RoomClass;

	TArray<TArray<FLevelRoom>> LevelMap;

	FIntVector2 CentralRoomLocation;

	TArray<FIntVector2> GeneratedRoomsLocations;

	int GeneratedRoomsAmount;
	int ChosenRoomsAmount;
};
