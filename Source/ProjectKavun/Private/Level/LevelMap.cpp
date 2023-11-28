// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/LevelMap.h"
#include "Rooms/RoomBase.h"

FLevelMap::FLevelMap(uint16 MapWidth, uint16 MapHeight)
	: Width(MapWidth),
	  Height(MapHeight),
	  Error(false, true, ERoomType::Default, ERoomShape::Square, nullptr, -1)
{
	LevelMap.SetNum(Height);
	for ( auto& LevelMapRow : LevelMap )
	{
		LevelMapRow.Init({false, true, ERoomType::Default, ERoomShape::Square, 0}, Width);
	}
}

int FLevelMap::CountOccupiedNeighbors(const FIntPoint& Location) const
{
	if ( !IsInBounds(Location) )
	{
		return 0;
	}

	int Count = 0;

	Count += IsOccupiedSafe(Location + KIntPointUp);
	Count += IsOccupiedSafe(Location + KIntPointDown);
	Count += IsOccupiedSafe(Location + KIntPointRight);
	Count += IsOccupiedSafe(Location + KIntPointLeft);

	return Count;
}

bool FLevelMap::GetNeighbors(const FIntPoint& Location, TMap<FIntPoint, FLevelRoom>& Neighbors) const
{
	UE_LOG(LogTemp, Display, TEXT("Getting neighbors for %s"), *Location.ToString());

	if ( !IsInBounds(Location) )
	{
		return false;
	}

	Neighbors.Add(KIntPointUp, GetSafe(Location + KIntPointUp));
	Neighbors.Add(KIntPointDown, GetSafe(Location + KIntPointDown));
	Neighbors.Add(KIntPointRight, GetSafe(Location + KIntPointRight));
	Neighbors.Add(KIntPointLeft, GetSafe(Location + KIntPointLeft));

	return true;
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
