// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/LevelMap.h"
#include "Rooms/RoomBase.h"

const FIntVector2 KIntVector2Up    = {0, 1};
const FIntVector2 KIntVector2Down  = {0, -1};
const FIntVector2 KIntVector2Left  = {-1, 0};
const FIntVector2 KIntVector2Right = {1, 0};

FLevelMap::FLevelMap(uint16 MapWidth, uint16 MapHeight)
	: Width(MapWidth),
	  Height(MapHeight),
	  Error(false, true, ERoomType::Default, ERoomShape::Square, -1)
{
	LevelMap.SetNum(Height);
	for ( auto& LevelMapRow : LevelMap )
	{
		LevelMapRow.Init({false, true, ERoomType::Default, ERoomShape::Square, 0}, Width);
	}
}

int FLevelMap::CountOccupiedNeighbours(const FIntVector2& Location) const
{
	if ( !IsInBounds(Location) )
	{
		return 0;
	}

	int Count = 0;

	Count += IsOccupiedSafe(Location + KIntVector2Up);
	Count += IsOccupiedSafe(Location + KIntVector2Down);
	Count += IsOccupiedSafe(Location + KIntVector2Right);
	Count += IsOccupiedSafe(Location + KIntVector2Left);

	return Count;
}

TMap<FIntVector2, FLevelRoom> FLevelMap::GetNeighbors(const FIntVector2& Location) const
{
	TMap<FIntVector2, FLevelRoom> Neighbors;

	if ( !IsInBounds(Location) )
	{
		return Neighbors;
	}

	Neighbors[KIntVector2Up]    = GetSafe(Location + KIntVector2Up);
	Neighbors[KIntVector2Down]  = GetSafe(Location + KIntVector2Down);
	Neighbors[KIntVector2Right] = GetSafe(Location + KIntVector2Right);
	Neighbors[KIntVector2Left]  = GetSafe(Location + KIntVector2Left);

	return Neighbors;
}

void FLevelMap::PrintInConsole()
{
	FString LevelString;
	for ( int Y = 0; Y < LevelMap.Num(); ++Y )
	{
		for ( int X = 0; X < LevelMap[Y].Num(); ++X )
		{
			const FLevelRoom& LevelRoom = LevelMap[Y][X];
			if ( LevelRoom.bOccupied )
			{
				LevelString.Append(FString::Printf(TEXT("%02d "), LevelRoom.Id));
			}
			else
			{
				LevelString.Append("   ");
			}
		}
		LevelString.Append("\n");
	}
	UE_LOG(LogTemp, Display, TEXT("LevelMap:\n%s"), *LevelString);
}
