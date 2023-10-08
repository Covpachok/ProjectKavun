// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

enum class ERoomShape : uint8;
enum class ERoomType : uint8;

struct FLevelRoom
{
	bool bOccupied   = false;
	bool bOriginTile = true;

	ERoomType  RoomType;
	ERoomShape RoomShape;

	uint16 Id = 0;
};

const FIntPoint KIntPointUp    = {0, 1};
const FIntPoint KIntPointDown  = {0, -1};
const FIntPoint KIntPointLeft  = {-1, 0};
const FIntPoint KIntPointRight = {1, 0};

class PROJECTKAVUN_API FLevelMap
{
public:
	FLevelMap(uint16 MapWidth, uint16 MapHeight);

	int                           CountOccupiedNeighbours(const FIntPoint& Location) const;
	bool GetNeighbors(const FIntPoint& Location, TMap<FIntPoint, FLevelRoom> &Neighbors) const;

	void PrintInConsole();

	/* Getters */
	uint16 GetWidth() const { return Width; }
	uint16 GetHeight() const { return Height; }
	
	const FLevelRoom& GetSafe(const FIntPoint& Location) const
	{
		return IsInBounds(Location) ? LevelMap[Location.Y][Location.X] : Error;
	}

	const FLevelRoom& Get(const FIntPoint& Location) const
	{
		return LevelMap[Location.Y][Location.X];
	}

	FLevelRoom& AtSafe(const FIntPoint& Location)
	{
		return IsInBounds(Location) ? LevelMap[Location.Y][Location.X] : Error;
	}

	FLevelRoom& At(const FIntPoint& Location)
	{
		return LevelMap[Location.Y][Location.X];
	}

	/* Bool Return Functions */
	bool IsInBounds(const FIntPoint& Location) const
	{
		return Location.X >= 0 && Location.X < Width && Location.Y >= 0 && Location.Y < Height;
	}

	bool IsOccupiedSafe(const FIntPoint& Location) const
	{
		return IsInBounds(Location) ? LevelMap[Location.Y][Location.X].bOccupied : false;
	}

	bool IsOccupied(const FIntPoint& Location) const
	{
		return LevelMap[Location.Y][Location.X].bOccupied;
	}

private:
	TArray<TArray<FLevelRoom>> LevelMap;

	uint16 Width;
	uint16 Height;

	FLevelRoom Error;
};
