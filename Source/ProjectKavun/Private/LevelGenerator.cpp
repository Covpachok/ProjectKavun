// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"

#include "RoomsManager.h"
#include "DSP/LFO.h"

ALevelGenerator::ALevelGenerator()
{
	MaxLevelHeight       = 11;
	MaxLevelWidth        = 11;
	RoomDelta            = {1200, 1800, 0};
	MinRoomsAmount       = 5;
	MaxRoomsAmount       = 10;
	GeneratedRoomsAmount = 0;
	MaxBigRooms          = 1;
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
	LevelMap.Empty();
	GeneratedRoomsLocations.Empty();
	GeneratedRoomsShapes.Empty();

	BigRoomsPlaced       = 0;
	GeneratedRoomsAmount = 0;
	ChosenRoomsAmount    = 0;

	LevelMap.SetNum(MaxLevelHeight);
	for ( auto& LevelMapRow : LevelMap )
	{
		LevelMapRow.Init({false, 0}, MaxLevelWidth);
	}

	CentralRoomLocation = {MaxLevelWidth / 2, MaxLevelHeight / 2};
	ChosenRoomsAmount   = FMath::RandRange(MinRoomsAmount, MaxRoomsAmount);

	// Starting Room
	GenerateRoom(CentralRoomLocation, ERoomShape::Square, false);

	for ( int i = 0; i < GeneratedRoomsLocations.Num() && GeneratedRoomsAmount < ChosenRoomsAmount; ++i )
	{
		GenerateNeighborFor(GeneratedRoomsLocations[i], GeneratedRoomsShapes[i]);
	}
	// for ( int j = 0; j < 10; ++j )
	// {
	// 	for ( int i = 0; GeneratedRoomsAmount < ChosenRoomsAmount && i < GeneratedRoomsLocations.Num(); ++i )
	// 	{
	// 		FIntVector2 RoomLocation = GeneratedRoomsLocations[i];
	// 		FIntVector2 NewRoomLocation;
	//
	// 		int Attempts = 0;
	// 		while ( !ChooseUnoccupiedNeighbor(RoomLocation, NewRoomLocation) )
	// 		{
	// 			if ( Attempts > 100 )
	// 			{
	// 				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow,
	// 				                                 FString::Printf(TEXT("[%d] Too many attempts :("), i));
	// 				break;
	// 			}
	// 			++Attempts;
	// 		}
	// 		if ( Attempts > 100 )
	// 		{
	// 			continue;
	// 		}
	//
	// 		if ( CountNeighbours(NewRoomLocation) > 1 )
	// 		{
	// 			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow,
	// 			                                 FString::Printf(TEXT("[%d] Too many neighbors :("), i));
	// 			continue;
	// 		}
	//
	// 		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("Radical measures")));
	// 		GenerateRoom(NewRoomLocation, ERoomShape::Square);
	// 	}
	// }

	if ( RoomsManager == nullptr )
	{
		return;
	}

	int PrevRoomId = -1;
	for ( int i = 0; i < GeneratedRoomsLocations.Num(); ++i )
	{
		FIntVector2 RoomLocation;
		RoomLocation = GeneratedRoomsLocations[i];

		FLevelRoom& Room          = LevelMap[RoomLocation.Y][RoomLocation.X];
		if(PrevRoomId == Room.Id)
		{
			continue;
		}
		
		FVector     SpawnLocation = {
				RoomDelta.X * (RoomLocation.X - CentralRoomLocation.X),
				RoomDelta.Y * (RoomLocation.Y - CentralRoomLocation.Y),
				0
		};

		RoomsManager->SpawnRoom(SpawnLocation, GeneratedRoomsShapes[i], ERoomType::Default);
		PrevRoomId = Room.Id;
	}
}

void ALevelGenerator::GenerateRoom(const FIntVector2& Location, ERoomShape RoomShape, bool bCanGiveUp)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green,
	                                 FString::Printf(TEXT("Room(%d) generated at: [x: %d | y: %d]"),
	                                                 GeneratedRoomsAmount, Location.X, Location.Y));

	++GeneratedRoomsAmount;

	const FRoomShapeDetails& Details = GRoomShapeDetails[static_cast<uint8>(RoomShape)];
	if ( Details.OccupiedTilesAmount > 1 )
	{
		++BigRoomsPlaced;
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
		Room.Occupied    = true;
		Room.Id          = GeneratedRoomsAmount;

		GeneratedRoomsLocations.Add(Loc);
		GeneratedRoomsShapes.Add(RoomShape);
	}

	if ( bCanGiveUp )
	{
		if ( FMath::RandRange(1, 2) == 1 || CountNeighbours(Location) > 1 )
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Gave up on neighbors")));
			return;
		}
	}

	GenerateNeighborFor(Location, RoomShape);
}

void ALevelGenerator::GenerateNeighborFor(const FIntVector2& OriginLocation, ERoomShape OriginShape)
{
	// Doesn't allow to not spawn a neighbour if not enough rooms been generated
	int       Low              = (GeneratedRoomsAmount == 0) + (GeneratedRoomsAmount < ChosenRoomsAmount);
	const int NeighboursAmount = FMath::RandRange(Low, 3);

	for ( int i = 0; i < NeighboursAmount && GeneratedRoomsAmount < ChosenRoomsAmount; ++i )
	{
		FIntVector2 NewRoomLocation;

		bool bLocationIsOccupied = true;
		for ( int j = 0; j < 100 && bLocationIsOccupied; ++j )
		{
			if ( !ChooseUnoccupiedNeighbor(OriginLocation, NewRoomLocation) )
			{
				continue;
			}

			// Too many neighbors already? Skip
			if ( CountNeighbours(NewRoomLocation) > 1 )
			{
				bLocationIsOccupied = true;
			}
			else
			{
				bLocationIsOccupied = LevelMap[NewRoomLocation.Y][NewRoomLocation.X].Occupied;
			}
		}
		if ( bLocationIsOccupied )
		{
			continue;
		}

		/* chance that room will be of a different shape */
		ERoomShape NewRoomShape = ERoomShape::Square;
		{
			FIntVector2 Correction{0, 0};
			bool        bBadShape = (FMath::RandRange(1, 2) == 1);
			for ( int j = 0; j < 50 && bBadShape; ++j )
			{
				ERoomShape RandomShape = static_cast<ERoomShape>(FMath::RandRange(
						1, 5));//static_cast<uint8>(ERoomShape::Amount) - 1));

				if ( CanPlaceRoomShape(NewRoomLocation, RandomShape, Correction) )
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

	bool Found = false;
	while ( !Found && !Neighbors.IsEmpty() )
	{
		int Index        = FMath::RandRange(0, Neighbors.Num() - 1);
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

	int Count = 0;

	Count += IsOccupiedSafe(Location + FIntVector2{0, 1});
	Count += IsOccupiedSafe(Location + FIntVector2{0, -1});
	Count += IsOccupiedSafe(Location + FIntVector2{1, 0});
	Count += IsOccupiedSafe(Location + FIntVector2{-1, 0});

	return Count;
}

bool ALevelGenerator::CanPlaceRoomShape(const FIntVector2& Location, ERoomShape Shape, FIntVector2& PossibleCorrection)
{
	if ( !IsInBounds(Location) )
	{
		return false;
	}

	const FRoomShapeDetails& Details   = GRoomShapeDetails[static_cast<uint8>(Shape)];
	bool                     bCanPlace = true;

	if ( Details.OccupiedTilesAmount > 1 && BigRoomsPlaced >= MaxBigRooms )
	{
		return false;
	}

	PossibleCorrection = {0, 0};
	if ( RoomShapeInteralCheck(Location, Details) )
	{
		return true;
	}

	if ( Details.OccupiedTilesAmount < 2 )
	{
		return false;
	}

	PossibleCorrection = {0, 1};
	if ( RoomShapeInteralCheck(Location + PossibleCorrection, Details) )
	{
		return true;
	}

	PossibleCorrection = {0, -1};
	if ( RoomShapeInteralCheck(Location + PossibleCorrection, Details) )
	{
		return true;
	}

	PossibleCorrection = {1, 0};
	if ( RoomShapeInteralCheck(Location + PossibleCorrection, Details) )
	{
		return true;
	}

	PossibleCorrection = {-1, 0};
	if ( RoomShapeInteralCheck(Location + PossibleCorrection, Details) )
	{
		return true;
	}

	PossibleCorrection = {0, 0};
	return false;
}

bool ALevelGenerator::RoomShapeInteralCheck(const FIntVector2& Location, FRoomShapeDetails ShapeDetails)
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

		if ( ShapeDetails.bLeftRightAccessible == false )
		{
			if ( IsOccupiedSafe(CheckLocation + FIntVector2{1, 0}) ||
			     IsOccupiedSafe(CheckLocation + FIntVector2{-1, 0}) )
			{
				bSuccess = false;
				break;
			}
		}

		if ( ShapeDetails.bUpDownAccessible == false )
		{
			if ( IsOccupiedSafe(CheckLocation + FIntVector2{0, 1}) ||
			     IsOccupiedSafe(CheckLocation + FIntVector2{0, -1}) )
			{
				bSuccess = false;
				break;
			}
		}
	}
	return bSuccess;
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
