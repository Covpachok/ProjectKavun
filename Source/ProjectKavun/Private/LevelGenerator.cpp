// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"

#include "RoomsManager.h"
#include "Kismet/GameplayStatics.h"

const FIntVector2 KIntVector2Up    = {0, 1};
const FIntVector2 KIntVector2Down  = {0, -1};
const FIntVector2 KIntVector2Left  = {-1, 0};
const FIntVector2 KIntVector2Right = {1, 0};

ALevelGenerator::ALevelGenerator()
{
	MaxLevelHeight       = 11;
	MaxLevelWidth        = 11;
	RoomDelta            = {1200, 1800, 0};
	MinRoomsAmount       = 5;
	MaxRoomsAmount       = 7;
	GeneratedRoomsAmount = 0;
	MaxBigRooms          = 1;
	MaxNarrowRooms       = 1;
	bGenerateAtStart     = true;
}

void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();

	if ( bGenerateAtStart )
	{
		GenerateLevel();
		PrintLevel();
	}
}

void ALevelGenerator::GenerateLevel()
{
	float Time = UGameplayStatics::GetRealTimeSeconds(GetWorld());

	LevelMap.Empty();
	GeneratedRoomsLocations.Empty();

	BigRoomsPlaced       = 0;
	NarrowRoomsPlaced    = 0;
	GeneratedRoomsAmount = 0;
	ChosenRoomsAmount    = 0;

	LevelMap.SetNum(MaxLevelHeight);
	for ( auto& LevelMapRow : LevelMap )
	{
		LevelMapRow.Init({false, ERoomType::Default, ERoomShape::Square, 0}, MaxLevelWidth);
	}

	CentralRoomLocation = {MaxLevelWidth / 2, MaxLevelHeight / 2};
	ChosenRoomsAmount   = FMath::RandRange(MinRoomsAmount, MaxRoomsAmount);

	// Starting Room
	GenerateRoom(CentralRoomLocation, ERoomShape::Square, false);

	for ( int i = 0; i < GeneratedRoomsLocations.Num() && GeneratedRoomsAmount < ChosenRoomsAmount; ++i )
	{
		GenerateNeighborFor(GeneratedRoomsLocations[i]);
	}

	if ( RoomsManager == nullptr )
	{
		return;
	}

	int PrevRoomId = -1;
	for ( int i = 0; i < GeneratedRoomsLocations.Num(); ++i )
	{
		FIntVector2 RoomLocation;
		RoomLocation = GeneratedRoomsLocations[i];

		FLevelRoom& Room = LevelMap[RoomLocation.Y][RoomLocation.X];
		if ( PrevRoomId == Room.Id )
		{
			continue;
		}

		FVector SpawnLocation = {
				RoomDelta.X * (RoomLocation.X - CentralRoomLocation.X),
				RoomDelta.Y * (RoomLocation.Y - CentralRoomLocation.Y),
				0
		};

		RoomsManager->SpawnRoom(SpawnLocation, Room.RoomShape, Room.RoomType);
		PrevRoomId = Room.Id;
	}

	Time = UGameplayStatics::GetRealTimeSeconds(GetWorld()) - Time;
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green,
	                                 FString::Printf(TEXT("Time taken to generate: %.4f"), Time));
}

void ALevelGenerator::GenerateRoom(const FIntVector2& Location, ERoomShape RoomShape, bool bCanGiveUp)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green,
	                                 FString::Printf(TEXT("Room(%d) generated at: [x: %d | y: %d]"),
	                                                 GeneratedRoomsAmount, Location.X, Location.Y));

	++GeneratedRoomsAmount;

	const FRoomShapeDetails& Details = GRoomShapeDetails[RoomShape];
	if ( Details.ShapeType == ERoomShapeType::Big )
	{
		++BigRoomsPlaced;
	}
	if ( Details.ShapeType == ERoomShapeType::Narrow )
	{
		++NarrowRoomsPlaced;
	}

	for ( int i = 0; i < Details.OccupiedTilesAmount; ++i )
	{
		FIntVector2 Loc = Location + Details.OccupiedTilesLocations[i];
		if ( !IsInBounds(Loc) )
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green,
			                                 FString::Printf(TEXT("%d FUCKING WEIRD: [x: %d | y: %d]"),
			                                                 GeneratedRoomsAmount, Loc.X, Loc.Y));
			break;
		}
		FLevelRoom& Room = LevelMap[Loc.Y][Loc.X];
		Room.bOccupied   = true;
		Room.Id          = GeneratedRoomsAmount;
		Room.RoomShape   = RoomShape;

		GeneratedRoomsLocations.Add(Loc);
	}

	if ( bCanGiveUp )
	{
		if ( FMath::RandRange(1, 2) == 1 || CountNeighbours(Location) > 1 )
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Gave up on neighbors")));
			return;
		}
	}

	GenerateNeighborFor(Location);
}

void ALevelGenerator::GenerateNeighborFor(const FIntVector2& ForLocation)
{
	// Doesn't allow to not spawn a neighbour if not enough rooms been generated
	int       Low              = (GeneratedRoomsAmount == 0) + (GeneratedRoomsAmount < ChosenRoomsAmount);
	const int NeighboursAmount = FMath::RandRange(Low, 3);

	for ( int i = 0; i < NeighboursAmount && GeneratedRoomsAmount < ChosenRoomsAmount; ++i )
	{
		FIntVector2 NewRoomLocation;

		if ( !FindUnoccupiedNeighbor(ForLocation, NewRoomLocation) )
		{
			// Not found any unoccupied neighbor
			continue;
		}

		if ( CountNeighbours(NewRoomLocation) > 1 )
		{
			// Too many neighbors
			continue;
		}

		ERoomShape NewRoomShape = ERoomShape::Square;
		/* Chance that room will be of a different shape */
		if ( NarrowRoomsPlaced < MaxNarrowRooms || BigRoomsPlaced < MaxBigRooms )
		{
			FIntVector2 Correction{0, 0};
			bool        bBadShape = (FMath::RandRange(1, 2) == 1);

			// Should do random pick from an array instead of brute force picking
			for ( int j = 0; j < 50 && bBadShape; ++j )
			{
				ERoomShape RandomShape = static_cast<ERoomShape>(FMath::RandRange(
						1, DebugUpperShape)); //static_cast<uint8>(ERoomShape::Amount) - 1));

				if ( CanPlaceRoom(NewRoomLocation, RandomShape, Correction) )
				{
					NewRoomShape = RandomShape;
					bBadShape    = false;
				}
			}
			NewRoomLocation += Correction;
		}

		/* Another bounds check just in case */
		if ( IsInBounds(NewRoomLocation) )
		{
			GenerateRoom(NewRoomLocation, NewRoomShape);
		}
	}
}

bool ALevelGenerator::FindUnoccupiedNeighbor(const FIntVector2& ForLocation, FIntVector2& ReturnNeighborLocation) const
{
	if ( !IsInBounds(ForLocation) )
	{
		return false;
	}

	FLevelRoom        RoomInfo     = LevelMap[ForLocation.Y][ForLocation.X];
	FRoomShapeDetails ShapeDetails = GRoomShapeDetails[RoomInfo.RoomShape];

	TArray<FIntVector2> Neighbors{};

	if ( ShapeDetails.bLeftRightAccessible )
	{
		Neighbors.Add(ForLocation + KIntVector2Left);
		Neighbors.Add(ForLocation + KIntVector2Right);
	}
	if ( ShapeDetails.bUpDownAccessible )
	{
		Neighbors.Add(ForLocation + KIntVector2Down);
		Neighbors.Add(ForLocation + KIntVector2Up);
	}

	bool Found = false;
	while ( !Found && !Neighbors.IsEmpty() )
	{
		int Index              = FMath::RandRange(0, Neighbors.Num() - 1);
		ReturnNeighborLocation = Neighbors[Index];
		Neighbors.RemoveAt(Index);

		if ( IsInBounds(ReturnNeighborLocation) )
		{
			Found = !LevelMap[ReturnNeighborLocation.Y][ReturnNeighborLocation.X].bOccupied;
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

	int Count = 0;

	Count += IsOccupiedSafe(Location + KIntVector2Up);
	Count += IsOccupiedSafe(Location + KIntVector2Down);
	Count += IsOccupiedSafe(Location + KIntVector2Right);
	Count += IsOccupiedSafe(Location + KIntVector2Left);

	return Count;
}

bool ALevelGenerator::CanPlaceRoom(const FIntVector2& Location, ERoomShape Shape, FIntVector2& Correction)
{
	if ( !IsInBounds(Location) )
	{
		return false;
	}


	const FRoomShapeDetails& Details   = GRoomShapeDetails[Shape];
	bool                     bCanPlace = true;

	if ( (Details.ShapeType == ERoomShapeType::Big && BigRoomsPlaced >= MaxBigRooms) ||
	     (Details.ShapeType == ERoomShapeType::Narrow && NarrowRoomsPlaced >= MaxNarrowRooms) )
	{
		return false;
	}

	Correction = {0, 0};
	if ( CanBePlacedAt(Location, Details) )
	{
		return true;
	}

	// Will not try to correct if room is 1 tile sized
	if ( Details.OccupiedTilesAmount < 2 )
	{
		return false;
	}

	const FIntVector2 Corrections[]{{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
	for ( int i = 0; i < 4; ++i )
	{
		Correction = Corrections[i];
		if ( CanBePlacedAt(Location + Correction, Details) )
		{
			// Prevent from spawning a room without connections
			if ( CountNeighbours(Location + Correction) != 0 )
			{
				return true;
			}
		}
	}

	Correction = {0, 0};
	return false;
}

bool ALevelGenerator::CanBePlacedAt(const FIntVector2& Location, FRoomShapeDetails ShapeDetails)
{
	bool bSuccess = true;
	for ( int i = 0; i < ShapeDetails.OccupiedTilesAmount; ++i )
	{
		const FIntVector2 CheckLocation = ShapeDetails.OccupiedTilesLocations[i] + Location;
		if ( IsOccupiedSafe(CheckLocation) )
		{
			bSuccess = false;
			break;
		}

		if ( ShapeDetails.bLeftRightAccessible == false && (
			     IsOccupiedSafe(CheckLocation + KIntVector2Right) ||
			     IsOccupiedSafe(CheckLocation + KIntVector2Left)) )
		{
			bSuccess = false;
			break;
		}

		if ( ShapeDetails.bUpDownAccessible == false && (
			     IsOccupiedSafe(CheckLocation + KIntVector2Up) ||
			     IsOccupiedSafe(CheckLocation + KIntVector2Down)) )
		{
			bSuccess = false;
			break;
		}

		bSuccess = CanBePlacedNear(CheckLocation, CheckLocation + KIntVector2Up) &&
		           CanBePlacedNear(CheckLocation, CheckLocation + KIntVector2Down) &&
		           CanBePlacedNear(CheckLocation, CheckLocation + KIntVector2Right) &&
		           CanBePlacedNear(CheckLocation, CheckLocation + KIntVector2Left);

		if ( !bSuccess )
		{
			break;
		}
	}

	return
			bSuccess;
}

bool ALevelGenerator::CanBePlacedNear(const FIntVector2& PlaceLocation, const FIntVector2& NeighborLocation)
{
	if ( !IsInBounds(NeighborLocation) || !IsInBounds(PlaceLocation) )
	{
		return false;
	}

	// Surely can be placed near empty tile
	if ( !IsOccupied(NeighborLocation) )
	{
		return true;
	}

	const FLevelRoom&        NeighborRoom = LevelMap[NeighborLocation.Y][NeighborLocation.X];
	const FIntVector2        LocationDiff = PlaceLocation - NeighborLocation;
	const FRoomShapeDetails& ShapeDetails = GRoomShapeDetails[NeighborRoom.RoomShape];

	return !((!ShapeDetails.bLeftRightAccessible && FMath::Abs(LocationDiff.X) == 1) ||
	         (!ShapeDetails.bUpDownAccessible && FMath::Abs(LocationDiff.Y) == 1));
}

void ALevelGenerator::PrintLevel()
{
	FString LevelString;
	for ( int Y = 0; Y < LevelMap.Num(); ++Y )
	{
		for ( int X = 0; X < LevelMap[Y].Num(); ++X )
		{
			if ( LevelMap[Y][X].bOccupied )
			{
				LevelString.Append(FString::Printf(TEXT("%02d]"), LevelMap[Y][X].Id));
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
