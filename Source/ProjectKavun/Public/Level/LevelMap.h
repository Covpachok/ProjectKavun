// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Rooms/RoomBase.h"

struct FLevelRoom
{
	bool bOccupied   = false;
	bool bOriginTile = true;

	ERoomType  RoomType;
	ERoomShape RoomShape;

	uint16 Id = 0;
};

class PROJECTKAVUN_API FLevelMap
{
public:
	FLevelMap(uint16 MapWidth, uint16 MapHeight);

	int                           CountOccupiedNeighbours(const FIntVector2& Location) const;
	TMap<FIntVector2, FLevelRoom> GetNeighbors(const FIntVector2& Location) const;

	void PrintInConsole();

	/* Getters */
	uint16 GetWidth() const { return Width; }
	uint16 GetHeight() const { return Height; }
	
	const FLevelRoom& GetSafe(const FIntVector2& Location) const
	{
		return IsInBounds(Location) ? LevelMap[Location.Y][Location.X] : Error;
	}

	const FLevelRoom& Get(const FIntVector2& Location) const
	{
		return LevelMap[Location.Y][Location.X];
	}

	FLevelRoom& AtSafe(const FIntVector2& Location)
	{
		return IsInBounds(Location) ? LevelMap[Location.Y][Location.X] : Error;
	}

	FLevelRoom& At(const FIntVector2& Location)
	{
		return LevelMap[Location.Y][Location.X];
	}

	/* Bool Return Functions */
	bool IsInBounds(const FIntVector2& Location) const
	{
		return Location.X >= 0 && Location.X < Width && Location.Y >= 0 && Location.Y < Height;
	}

	bool IsOccupiedSafe(const FIntVector2& Location) const
	{
		return IsInBounds(Location) ? LevelMap[Location.Y][Location.X].bOccupied : false;
	}

	bool IsOccupied(const FIntVector2& Location) const
	{
		return LevelMap[Location.Y][Location.X].bOccupied;
	}

private:
	TArray<TArray<FLevelRoom>> LevelMap;

	uint16 Width;
	uint16 Height;

	FLevelRoom Error;
};
