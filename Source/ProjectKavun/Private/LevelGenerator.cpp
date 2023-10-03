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

	PrintLevel();
}

void ALevelGenerator::GenerateLevel()
{
	ChosenRoomsAmount = FMath::RandRange(MinRoomsAmount, MaxRoomsAmount);
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue,
	                                 FString::Printf(TEXT("Chosen rooms amount: %d"), ChosenRoomsAmount));

	GenerateRoom(CentralRoomLocation);

	// Not enough rooms has been generated, so we'll try to generate more
	for ( int j = 0; j < 10; ++j )
	{
		for ( int i = 0; GeneratedRoomsAmount < ChosenRoomsAmount && i < GeneratedRoomsLocations.Num(); ++i )
		{
			FIntVector2 RoomLocation = GeneratedRoomsLocations[i];
			FIntVector2 NewRoomLocation;

			int Attempts = 0;
			while ( !ChooseUnoccupiedNeighbor(RoomLocation, NewRoomLocation) )
			{
				if ( Attempts > 100 )
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("[%d] Too many attempts :("), i));
					break;
				}
				++Attempts;
			}
			if ( Attempts > 100 )
			{
				continue;
			}

			if ( CountNeighbours(NewRoomLocation) > 1 )
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("[%d] Too many neighbors :("), i));
				continue;
			}

			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("Radical measures")));
			GenerateRoom(NewRoomLocation);
		}
	}

	for(int i = 0; i < GeneratedRoomsLocations.Num(); ++i)
	{
		FIntVector2 RoomLocation;
		RoomLocation = GeneratedRoomsLocations[i];

		FLevelRoom& Room = LevelMap[RoomLocation.Y][RoomLocation.X];
		SpawnRoom(RoomLocation);
	}
}

void ALevelGenerator::GenerateRoom(const FIntVector2& Location)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green,
	                                 FString::Printf(TEXT("Room(%d) generated at: [x: %d | y: %d]"),
	                                                 GeneratedRoomsAmount, Location.X, Location.Y));

	++GeneratedRoomsAmount;

	FLevelRoom& Room = LevelMap[Location.Y][Location.X];
	Room.Occupied    = true;
	Room.Id = GeneratedRoomsAmount;

	GeneratedRoomsLocations.Add(Location);

	// 33% chance to not generate anything
	if ( FMath::RandRange(1, 2) == 1 )
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
	if ( GeneratedRoomsAmount < ChosenRoomsAmount )
	{
		Low = 1;
	}

	const int NeighboursAmount = FMath::RandRange(Low, 3);

	int Attempt = 0;
	for ( int i = 0; i < NeighboursAmount && GeneratedRoomsAmount < ChosenRoomsAmount; ++i )
	{
		// GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Attempt #%d"), Attempt));

		FIntVector2 NewRoomLocation;

		bool bOccupied = true;
		bool bSpawn    = true;
		while ( bOccupied )
		{
			// Idk why it might be possible, but in case of infinite loop that'll help
			if ( Attempt > 100 )
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Attempts > 100")));
				bSpawn = false;
				return;
			}

			if ( !ChooseUnoccupiedNeighbor(Location, NewRoomLocation) )
			{
				continue;
			}

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
		if ( IsInBounds(NewRoomLocation) && bSpawn )
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

	TArray<FIntVector2> Neighbors = {
			ForLocation + FIntVector2{0, 1},
			ForLocation + FIntVector2{0, -1},
			ForLocation + FIntVector2{-1, 0},
			ForLocation + FIntVector2{1, 0}
	};

	bool Found   = false;
	while ( !Found && !Neighbors.IsEmpty())
	{
		int Index = FMath::RandRange(0, Neighbors.Num() - 1);
		NeighborLocation = Neighbors[Index];
		Neighbors.RemoveAt(Index);
		
		if ( IsInBounds(NeighborLocation) )
		{
			Found = !LevelMap[NeighborLocation.Y][NeighborLocation.X].Occupied;
		}
	}

	return Found;
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

void ALevelGenerator::PrintLevel()
{
	FString LevelString;
    	for ( int Y = 0; Y < LevelMap.Num(); ++Y )
    	{
    		for ( int X = 0; X < LevelMap[Y].Num(); ++X )
    		{
    			if ( LevelMap[Y][X].Occupied )
    			{
    				LevelString.Append(FString::Printf(TEXT("%02d "), LevelMap[Y][X].Id));
    			}
    			else
    			{
    				LevelString.Append(".. ");
    			}
    		}
    		LevelString.Append("\n");
    	}
    	UE_LOG(LogTemp, Display, TEXT("LevelMap:\n%s"), *LevelString);
}
