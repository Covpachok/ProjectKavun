// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"

#include "RoomsManager.h"
#include "Level/LevelMap.h"
#include "Rooms/RoomBase.h"

const FIntVector2 KIntVector2Up    = {0, 1};
const FIntVector2 KIntVector2Down  = {0, -1};
const FIntVector2 KIntVector2Left  = {-1, 0};
const FIntVector2 KIntVector2Right = {1, 0};

ALevelGenerator::ALevelGenerator()
	: bGenerateAtStart(true),
	  LevelWidth(11),
	  LevelHeight(11),
	  MinRoomsCount(5),
	  MaxRoomsCount(7),
	  MaxBigRooms(1),
	  MaxNarrowRooms(1),
	  RoomsManager(nullptr),
	  DebugUpperShape(11),
	  GeneratedRoomsCount(0),
	  TargetRoomsCount(0),
	  BigRoomsPlaced(0),
	  NarrowRoomsPlaced(0)
{
}

void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();

	if ( bGenerateAtStart )
	{
		GenerateLevel();
		LevelMap->PrintInConsole();
	}
}

void ALevelGenerator::GenerateLevel()
{
	LevelMap = MakeShared<FLevelMap>(LevelWidth, LevelHeight);
	GeneratedRoomLocations.Empty();

	BigRoomsPlaced      = 0;
	NarrowRoomsPlaced   = 0;
	GeneratedRoomsCount = 0;
	TargetRoomsCount    = 0;

	const FIntVector2 CentralRoomLocation = {LevelWidth / 2, LevelHeight / 2};
	TargetRoomsCount                      = FMath::RandRange(MinRoomsCount, MaxRoomsCount);

	// Starting Room
	GenerateRoom(CentralRoomLocation, ERoomShape::Square, false);

	for ( int i = 0; i < GeneratedRoomLocations.Num() && GeneratedRoomsCount < TargetRoomsCount; ++i )
	{
		GenerateNeighborFor(GeneratedRoomLocations[i]);
	}

	if ( RoomsManager == nullptr )
	{
		return;
	}

	RoomsManager->OnLevelGenerationCompleted(*LevelMap, GeneratedRoomLocations);
}

void ALevelGenerator::GenerateRoom(const FIntVector2& Location, ERoomShape RoomShape, bool bCanGiveUp)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green,
	                                 FString::Printf(TEXT("Room(%d) generated at: [x: %d | y: %d]"),
	                                                 GeneratedRoomsCount, Location.X, Location.Y));

	++GeneratedRoomsCount;

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
		if ( !LevelMap->IsInBounds(Loc) )
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green,
			                                 FString::Printf(TEXT("%d FUCKING WEIRD: [x: %d | y: %d]"),
			                                                 GeneratedRoomsCount, Loc.X, Loc.Y));
			break;
		}
		FLevelRoom& Room = LevelMap->At(Loc);
		Room.bOccupied   = true;
		Room.bOriginTile = i == 0;
		Room.Id          = GeneratedRoomsCount;
		Room.RoomShape   = RoomShape;

		GeneratedRoomLocations.Add(Loc);
	}

	if ( bCanGiveUp )
	{
		if ( FMath::RandRange(1, 2) == 1 || LevelMap->CountOccupiedNeighbours(Location) > 1 )
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
	const int Low              = (GeneratedRoomsCount == 0) + (GeneratedRoomsCount < TargetRoomsCount);
	const int NeighboursAmount = FMath::RandRange(Low, 3);

	int NewRoomNeighborsAllowed = 1;
	if ( FMath::RandRange(1, 5) == 1 )
	{
		NewRoomNeighborsAllowed = 2;
	}

	for ( int i = 0; i < NeighboursAmount && GeneratedRoomsCount < TargetRoomsCount; ++i )
	{
		FIntVector2 NewRoomLocation;

		if ( !FindUnoccupiedNeighbor(ForLocation, NewRoomLocation) )
		{
			// Not found any unoccupied neighbor
			continue;
		}

		if ( LevelMap->CountOccupiedNeighbours(NewRoomLocation) > NewRoomNeighborsAllowed )
		{
			// Too many neighbors
			continue;
		}

		FIntVector2 Correction{0, 0};
		ERoomShape  NewRoomShape = ERoomShape::Square;
		if ( !CanPlaceRoom(NewRoomLocation, NewRoomShape, Correction) )
		{
			continue;
		}

		/* Chance that room will be of a different shape */
		if ( NarrowRoomsPlaced < MaxNarrowRooms || BigRoomsPlaced < MaxBigRooms )
		{
			bool bBadShape = (FMath::RandRange(1, 2) == 1);

			// Should do random pick from an array instead of brute force picking
			for ( int j = 0; j < 50 && bBadShape; ++j )
			{
				const ERoomShape RandomShape = static_cast<ERoomShape>(FMath::RandRange(
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
		if ( LevelMap->IsInBounds(NewRoomLocation) )
		{
			GenerateRoom(NewRoomLocation, NewRoomShape);
		}
	}
}

bool ALevelGenerator::FindUnoccupiedNeighbor(const FIntVector2& ForLocation, FIntVector2& ReturnNeighborLocation) const
{
	if ( !LevelMap->IsInBounds(ForLocation) )
	{
		return false;
	}

	const FLevelRoom        RoomInfo     = LevelMap->Get(ForLocation);
	const FRoomShapeDetails ShapeDetails = GRoomShapeDetails[RoomInfo.RoomShape];

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
		const int Index        = FMath::RandRange(0, Neighbors.Num() - 1);
		ReturnNeighborLocation = Neighbors[Index];
		Neighbors.RemoveAt(Index);

		if ( LevelMap->IsInBounds(ReturnNeighborLocation) )
		{
			Found = !LevelMap->At(ReturnNeighborLocation).bOccupied;
		}
	}

	return Found;
}

bool ALevelGenerator::CanPlaceRoom(const FIntVector2& Location, ERoomShape Shape, FIntVector2& Correction)
{
	if ( !LevelMap->IsInBounds(Location) )
	{
		return false;
	}


	const FRoomShapeDetails& Details   = GRoomShapeDetails[Shape];

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
			if ( LevelMap->CountOccupiedNeighbours(Location + Correction) != 0 )
			{
				return true;
			}
		}
	}

	Correction = {0, 0};
	return false;
}

bool ALevelGenerator::CanBePlacedAt(const FIntVector2& Location, const FRoomShapeDetails& ShapeDetails)
{
	bool bSuccess = true;
	for ( int i = 0; i < ShapeDetails.OccupiedTilesAmount; ++i )
	{
		const FIntVector2 CheckLocation = ShapeDetails.OccupiedTilesLocations[i] + Location;
		if ( LevelMap->IsOccupiedSafe(CheckLocation) )
		{
			bSuccess = false;
			break;
		}

		if ( ShapeDetails.bLeftRightAccessible == false && (
			     LevelMap->IsOccupiedSafe(CheckLocation + KIntVector2Right) ||
			     LevelMap->IsOccupiedSafe(CheckLocation + KIntVector2Left)) )
		{
			bSuccess = false;
			break;
		}

		if ( ShapeDetails.bUpDownAccessible == false && (
			     LevelMap->IsOccupiedSafe(CheckLocation + KIntVector2Up) ||
			     LevelMap->IsOccupiedSafe(CheckLocation + KIntVector2Down)) )
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

bool ALevelGenerator::CanBePlacedNear(const FIntVector2& PlaceLocation, const FIntVector2& NeighborLocation) const
{
	if ( !LevelMap->IsInBounds(NeighborLocation) || !LevelMap->IsInBounds(PlaceLocation) )
	{
		return false;
	}

	// Surely can be placed near empty tile
	if ( !LevelMap->IsOccupied(NeighborLocation) )
	{
		return true;
	}

	const FLevelRoom&        NeighborRoom = LevelMap->Get(NeighborLocation);
	const FIntVector2        LocationDiff = PlaceLocation - NeighborLocation;
	const FRoomShapeDetails& ShapeDetails = GRoomShapeDetails[NeighborRoom.RoomShape];

	return !((!ShapeDetails.bLeftRightAccessible && FMath::Abs(LocationDiff.X) == 1) ||
	         (!ShapeDetails.bUpDownAccessible && FMath::Abs(LocationDiff.Y) == 1));
}
