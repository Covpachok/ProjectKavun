// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"

#include "RoomsManager.h"
#include "Kismet/GameplayStatics.h"
#include "Level/LevelMap.h"
#include "Rooms/RoomBase.h"

DEFINE_LOG_CATEGORY(LevelGeneratorLog);

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

	// if RoomsManager isn't set manually, search for it on a scene
	if ( !IsValid(RoomsManager) )
	{
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARoomsManager::StaticClass(), OutActors);

		if ( OutActors.IsEmpty() )
		{
			UE_LOG(LevelGeneratorLog, Error, TEXT("%s : RoomsManager not found."), __FUNCTIONW__);
			return;
		}

		for ( auto& Actor : OutActors )
		{
			RoomsManager = Cast<ARoomsManager>(Actor);
			if ( IsValid(RoomsManager) )
			{
				break;
			}
		}

		if ( !IsValid(RoomsManager) )
		{
			UE_LOG(LevelGeneratorLog, Error, TEXT("%s : RoomsManager is invalid."), __FUNCTIONW__);
			return;
		}
	}

	if ( bGenerateAtStart )
	{
		GenerateLevel();
		LevelMap->PrintInConsole();
		RoomsManager->OnLevelGenerationCompleted(*LevelMap, GeneratedRoomLocations);
	}
}

void ALevelGenerator::GenerateLevel()
{
	UE_LOG(LevelGeneratorLog, Display, TEXT("%s : Level generation started."), __FUNCTIONW__);
	const double StartTime = FPlatformTime::Seconds();

	LevelMap = MakeShared<FLevelMap>(LevelWidth, LevelHeight);
	GeneratedRoomLocations.Empty();

	BigRoomsPlaced      = 0;
	NarrowRoomsPlaced   = 0;
	GeneratedRoomsCount = 0;
	TargetRoomsCount    = 0;

	const FIntPoint CentralRoomLocation = {LevelWidth / 2, LevelHeight / 2};
	TargetRoomsCount                    = FMath::RandRange(MinRoomsCount, MaxRoomsCount);

	// Starting Room
	GenerateRoom(CentralRoomLocation, ERoomShape::Square, false);

	for ( int i = 0; i < GeneratedRoomLocations.Num() && GeneratedRoomsCount < TargetRoomsCount; ++i )
	{
		GenerateNeighborFor(GeneratedRoomLocations[i]);
	}

	GenerateSpecialRooms(ERoomShape::Square, ERoomType::Treasure, false);
	
	GenerateSpecialRooms(ERoomShape::Square, ERoomType::Treasure, true);


	const double EndTime = FPlatformTime::Seconds();
	UE_LOG(LevelGeneratorLog, Display,
	       TEXT("%s : Level generation completed in %f seconds."), __FUNCTIONW__, EndTime - StartTime);
}

void ALevelGenerator::GenerateRoom(const FIntPoint& Location, ERoomShape RoomShape, bool bCanGiveUp)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green,
	// FString::Printf(TEXT("Room(%d) generated at: [x: %d | y: %d]"),
	// GeneratedRoomsCount, Location.X, Location.Y));

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
		FIntPoint Loc = Location + Details.OccupiedTilesLocations[i];
		if ( !LevelMap->IsInBounds(Loc) )
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green,
			// FString::Printf(TEXT("%d FUCKING WEIRD: [x: %d | y: %d]"),
			// GeneratedRoomsCount, Loc.X, Loc.Y));
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
			//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Gave up on neighbors")));
			return;
		}
	}

	GenerateNeighborFor(Location);
}

void ALevelGenerator::GenerateSpecialRooms(ERoomShape RoomShape, ERoomType RoomType, bool bGenerateNearStart)
{
	const int Dir   = bGenerateNearStart ? 1 : -1;
	const int Start = bGenerateNearStart ? 0 : GeneratedRoomLocations.Num() - 1;
	const int End   = bGenerateNearStart ? GeneratedRoomLocations.Num() : -1;

	for ( int i = Start; i != End; i += Dir )
	{
		FIntPoint& Location = GeneratedRoomLocations[i];

		FIntPoint SpecialRoomLocation;
		if ( !FindUnoccupiedNeighbor(Location, SpecialRoomLocation) )
		{
			continue;
		}

		const int NeighborsCount = LevelMap->CountOccupiedNeighbours(SpecialRoomLocation);
		if ( NeighborsCount > 1 )
		{
			continue;
		}

		FIntPoint Correction;
		if ( !CanPlaceRoom(SpecialRoomLocation, RoomShape, Correction) )
		{
			continue;
		}

		++GeneratedRoomsCount;

		FLevelRoom& SpecialRoom = LevelMap->At(SpecialRoomLocation);

		SpecialRoom.Id = GeneratedRoomsCount;

		SpecialRoom.RoomType  = RoomType;
		SpecialRoom.RoomShape = RoomShape;

		SpecialRoom.bOccupied   = true;
		SpecialRoom.bOriginTile = true;

		GeneratedRoomLocations.Add(SpecialRoomLocation);

		UE_LOG(LevelGeneratorLog, Display,
		       TEXT("%s : Special room of type[%d] has been generated."), __FUNCTIONW__,
		       static_cast<int>(RoomType));

		return;
	}

	UE_LOG(LevelGeneratorLog, Warning,
	       TEXT("%s : Special room of type[%d] hasn't been generated."), __FUNCTIONW__,
	       static_cast<int>(RoomType));
}

void ALevelGenerator::GenerateNeighborFor(const FIntPoint& ForLocation)
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
		FIntPoint NewRoomLocation;

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

		FIntPoint  Correction{0, 0};
		ERoomShape NewRoomShape = ERoomShape::Square;
		if ( !CanPlaceRoom(NewRoomLocation, NewRoomShape, Correction) )
		{
			continue;
		}

		/* Chance that room will be of a different shape */
		if ( FMath::RandRange(1, 2) == 1 && (NarrowRoomsPlaced < MaxNarrowRooms || BigRoomsPlaced < MaxBigRooms) )
		{
			for ( int j = 0; j < 50; ++j )
			{
				const ERoomShape RandomShape = static_cast<ERoomShape>(FMath::RandRange(1, DebugUpperShape));

				if ( CanPlaceRoom(NewRoomLocation, RandomShape, Correction) )
				{
					NewRoomShape = RandomShape;
					NewRoomLocation += Correction;
					break;
				}
			}
		}

		/* Another bounds check just in case */
		if ( LevelMap->IsInBounds(NewRoomLocation) )
		{
			GenerateRoom(NewRoomLocation, NewRoomShape);
		}
	}
}

bool ALevelGenerator::FindUnoccupiedNeighbor(const FIntPoint& ForLocation, FIntPoint& ReturnNeighborLocation) const
{
	if ( !LevelMap->IsInBounds(ForLocation) )
	{
		return false;
	}

	const FLevelRoom        RoomInfo     = LevelMap->Get(ForLocation);
	const FRoomShapeDetails ShapeDetails = GRoomShapeDetails[RoomInfo.RoomShape];

	TArray<FIntPoint> Neighbors{};

	if ( ShapeDetails.bLeftRightAccessible )
	{
		Neighbors.Add(ForLocation + KIntPointLeft);
		Neighbors.Add(ForLocation + KIntPointRight);
	}
	if ( ShapeDetails.bUpDownAccessible )
	{
		Neighbors.Add(ForLocation + KIntPointDown);
		Neighbors.Add(ForLocation + KIntPointUp);
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

bool ALevelGenerator::CanPlaceRoom(const FIntPoint& Location, ERoomShape Shape, FIntPoint& Correction)
{
	if ( !LevelMap->IsInBounds(Location) )
	{
		return false;
	}


	const FRoomShapeDetails& Details = GRoomShapeDetails[Shape];

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

	const FIntPoint Corrections[]{{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
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

bool ALevelGenerator::CanBePlacedAt(const FIntPoint& Location, const FRoomShapeDetails& ShapeDetails)
{
	bool bSuccess = true;
	for ( int i = 0; i < ShapeDetails.OccupiedTilesAmount; ++i )
	{
		const FIntPoint CheckLocation = ShapeDetails.OccupiedTilesLocations[i] + Location;
		if ( LevelMap->IsOccupiedSafe(CheckLocation) )
		{
			bSuccess = false;
			break;
		}

		if ( ShapeDetails.bLeftRightAccessible == false && (
			     LevelMap->IsOccupiedSafe(CheckLocation + KIntPointRight) ||
			     LevelMap->IsOccupiedSafe(CheckLocation + KIntPointLeft)) )
		{
			bSuccess = false;
			break;
		}

		if ( ShapeDetails.bUpDownAccessible == false && (
			     LevelMap->IsOccupiedSafe(CheckLocation + KIntPointUp) ||
			     LevelMap->IsOccupiedSafe(CheckLocation + KIntPointDown)) )
		{
			bSuccess = false;
			break;
		}

		bSuccess = CanBePlacedNear(CheckLocation, CheckLocation + KIntPointUp) &&
		           CanBePlacedNear(CheckLocation, CheckLocation + KIntPointDown) &&
		           CanBePlacedNear(CheckLocation, CheckLocation + KIntPointRight) &&
		           CanBePlacedNear(CheckLocation, CheckLocation + KIntPointLeft);

		if ( !bSuccess )
		{
			break;
		}
	}

	return
			bSuccess;
}

bool ALevelGenerator::CanBePlacedNear(const FIntPoint& PlaceLocation, const FIntPoint& NeighborLocation) const
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
	const FIntPoint          LocationDiff = PlaceLocation - NeighborLocation;
	const FRoomShapeDetails& ShapeDetails = GRoomShapeDetails[NeighborRoom.RoomShape];

	return !((!ShapeDetails.bLeftRightAccessible && FMath::Abs(LocationDiff.X) == 1) ||
	         (!ShapeDetails.bUpDownAccessible && FMath::Abs(LocationDiff.Y) == 1));
}
