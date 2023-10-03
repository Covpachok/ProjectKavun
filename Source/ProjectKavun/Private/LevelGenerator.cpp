// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"

enum ERoomDirections { ERoomDirections_Up, ERoomDirections_Down, ERoomDirections_Left, ERoomDirections_Right };

ALevelGenerator::ALevelGenerator()
{
	RoomDelta            = {1200, 1800, 0};
	MinRoomsAmount       = 5;
	MaxRoomsAmount       = 10;
	GeneratedRoomsAmount = 0;
}

void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();

	LevelMap.SetNum(MaxLevelHeight);
	for ( auto& LevelMapRow : LevelMap )
	{
		LevelMapRow.Init({false, false, false, false, false}, MaxLevelWidth);
	}

	CentralRoomLocation = {MaxLevelWidth / 2, MaxLevelHeight / 2};
	GenerateLevel();
}

void ALevelGenerator::GenerateLevel()
{
	GenerateRoom(CentralRoomLocation);

	// Not enough rooms been generated 
	for ( int i = 0; GeneratedRoomsAmount < MinRoomsAmount; ++i )
	{
		FIntVector2 RoomLocation = GeneratedRoomsLocations[i];
		FIntVector2 NewRoomLocation;
		if ( ChooseUnoccupiedNeighbor(RoomLocation, NewRoomLocation) )
		{
			GenerateRoom(NewRoomLocation);
		}
	}

	while ( !GeneratedRoomsLocations.IsEmpty() )
	{
		FIntVector2 RoomLocation;
		RoomLocation = GeneratedRoomsLocations.Pop();

		FLevelRoom& Room = LevelMap[RoomLocation.Y][RoomLocation.X];
		SpawnRoom(RoomLocation);
	}
}

void ALevelGenerator::GenerateRoom(const FIntVector2& Location)
{
	GEngine->AddOnScreenDebugMessage(-1,
	                                 15.f,
	                                 FColor::Green,
	                                 FString::Printf(
			                                 TEXT("Room(%d) generated at: [x: %d | y: %d]"),
			                                 GeneratedRoomsAmount,
			                                 Location.X,
			                                 Location.Y));

	++GeneratedRoomsAmount;

	FLevelRoom& Room = LevelMap[Location.Y][Location.X];
	Room.Occupied    = true;

	GeneratedRoomsLocations.Add(Location);

	// 33% chance to not generate anything
	if ( FMath::RandRange(1, 3) == 3 )
	{
		return;
	}

	// It already has more than two neighbors, so NO
	if ( CountNeighbours(Location) > 1 )
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Too much neighbours")));
		return;
	}

	// Doesn't allow to not spawn a neighbour if not enough rooms been generated
	int Low = 0;
	if ( GeneratedRoomsAmount < MinRoomsAmount )
	{
		Low = 1;
	}

	const int NeighboursAmount = FMath::RandRange(Low, 3);

	int Attempt = 0;
	for ( int i = 0; i < NeighboursAmount && GeneratedRoomsAmount < MaxRoomsAmount; ++i )
	{
		// GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Attempt #%d"), Attempt));

		FIntVector2 NewRoomLocation;

		bool bOccupied = true;
		while ( bOccupied )
		{
			// Idk why it might be possible, but in case of infinite loop that'll help
			if ( Attempt > 100 )
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Attempts > 100")));
				return;
			}

			ChooseUnoccupiedNeighbor(Location, NewRoomLocation);

			// Too many neighbors already? Skip
			if ( CountNeighbours(NewRoomLocation) > 1 )
			{
				bOccupied = true;
			}
			else
			{
				bOccupied = LevelMap[NewRoomLocation.Y][NewRoomLocation.X].Occupied;
			}

			++Attempt;
		}

		// Another bounds check just in case
		if ( IsInBounds(NewRoomLocation) )
		{
			GenerateRoom(NewRoomLocation);
		}
	}
}

void ALevelGenerator::SpawnRoom(const FIntVector2& Location)
{
	FVector SpawnLocation = {
			RoomDelta.X * (Location.X - CentralRoomLocation.X),
			RoomDelta.Y * (Location.Y - CentralRoomLocation.Y),
			0
	};

	GetWorld()->SpawnActor<AActor>(RoomClass, SpawnLocation, FRotator::ZeroRotator);
}

bool ALevelGenerator::ChooseUnoccupiedNeighbor(const FIntVector2& ForLocation, FIntVector2& NeighborLocation) const
{
	if ( !IsInBounds(ForLocation) )
	{
		return false;
	}

	int Attempt = 0;
	do
	{
		if ( Attempt > 100 )
		{
			return false;
		}

		ERoomDirections Direction = static_cast<ERoomDirections>(FMath::RandRange(0, 3));
		switch ( Direction )
		{
			case ERoomDirections_Up:
				NeighborLocation = ForLocation + FIntVector2{0, 1};
				break;
			case ERoomDirections_Down:
				NeighborLocation = ForLocation + FIntVector2{0, -1};
				break;
			case ERoomDirections_Left:
				NeighborLocation = ForLocation + FIntVector2{-1, 0};
				break;
			case ERoomDirections_Right:
				NeighborLocation = ForLocation + FIntVector2{1, 0};
				break;
			default:
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("WHY")));
				break;
		}
		++Attempt;
	}
	while ( !IsInBounds(NeighborLocation) || LevelMap[NeighborLocation.Y][NeighborLocation.X].Occupied );

	return true;
}

int ALevelGenerator::CountNeighbours(const FIntVector2& Location)
{
	if ( !IsInBounds(Location) )
	{
		return 0;
	}

	const FIntVector2 Up    = Location + FIntVector2{0, 1};
	const FIntVector2 Down  = Location + FIntVector2{0, -1};
	const FIntVector2 Right = Location + FIntVector2{1, 0};
	const FIntVector2 Left  = Location + FIntVector2{-1, 0};

	int Count = 0;
	if ( IsInBounds(Up) )
	{
		Count += LevelMap[Up.Y][Up.X].Occupied;
	}
	if ( IsInBounds(Down) )
	{
		Count += LevelMap[Down.Y][Down.X].Occupied;
	}
	if ( IsInBounds(Right) )
	{
		Count += LevelMap[Right.Y][Right.X].Occupied;
	}
	if ( IsInBounds(Left) )
	{
		Count += LevelMap[Left.Y][Left.X].Occupied;
	}

	return Count;
}
