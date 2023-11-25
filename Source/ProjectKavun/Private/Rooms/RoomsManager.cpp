// Fill out your copyright notice in the Description page of Project Settings.


#include "Rooms/RoomsManager.h"

#include "Doors/Door.h"
#include "Level/LevelMap.h"
#include "Rooms/RoomDecorationBase.h"
#include "Rooms/WallComponent.h"
#include "Rooms/RoomDetails.h"

DEFINE_LOG_CATEGORY(RoomsManagerLog);

ARoomsManager::ARoomsManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARoomsManager::BeginPlay()
{
	Super::BeginPlay();
}

void ARoomsManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARoomsManager::OnLevelGenerationCompleted(FLevelMap& LevelMap, const TArray<FIntPoint>& RoomLocations)
{
	CentralRoomLocation   = {LevelMap.GetWidth() / 2, LevelMap.GetHeight() / 2};
	ARoomBase* RoomActor  = nullptr;
	int        PrevRoomId = -1;
	for ( int i = 0; i < RoomLocations.Num(); ++i )
	{
		FIntPoint RoomLocation;
		RoomLocation = RoomLocations[i];

		FLevelRoom& Room = LevelMap.At(RoomLocation);
		if ( !Room.bOriginTile )
		{
			if ( IsValid(RoomActor) && PrevRoomId == Room.Id )
			{
				Room.RoomActorRef = RoomActor;
			}

			continue;
		}

		FVector RoomSpawnLocation = MapToWorldRoomLocation(RoomLocation);

		RoomActor = SpawnRoom(Room, RoomSpawnLocation);
		if ( !IsValid(RoomActor) )
		{
			UE_LOG(RoomsManagerLog, Error, TEXT("%s : RoomActor is invalid"), __FUNCTIONW__);
			continue;
		}
		Room.RoomActorRef = RoomActor;
		ConstructRoom(*RoomActor, LevelMap, Room, RoomLocation);
		SpawnDecoration(*RoomActor, Room);

		PrevRoomId = Room.Id;

		RoomActor->OnConstructionCompleted();
	}
	PlaceDoors(LevelMap);
}

ARoomBase* ARoomsManager::SpawnRoom(const FLevelRoom& LevelRoom, const FVector& WorldLocation)
{
	if ( RoomClassesByShape.IsEmpty() || !RoomClassesByShape.Contains(LevelRoom.RoomShape) )
	{
		return nullptr;
	}

	ARoomBase* RoomActor;
	if ( LevelRoom.RoomType != ERoomType::Default )
	{
		RoomActor = GetWorld()->SpawnActor<ARoomBase>(RoomClassesByType[LevelRoom.RoomType], WorldLocation,
		                                              FRotator::ZeroRotator);
		UE_LOG(LevelGeneratorLog, Display, TEXT("%s : Special room spawned"), __FUNCTIONW__)
	}
	else
	{
		RoomActor = GetWorld()->SpawnActor<ARoomBase>(RoomClassesByShape[LevelRoom.RoomShape], WorldLocation,
		                                              FRotator::ZeroRotator);
	}

	// RoomShape should be defined by derived class
	// RoomActor->SetShape(LevelRoom.RoomShape);
	RoomActor->SetType(LevelRoom.RoomType);

	return RoomActor;
}

void ARoomsManager::ConstructRoom(ARoomBase&       RoomActor, const FLevelMap& LevelMap, const FLevelRoom& LevelRoom,
                                  const FIntPoint& LevelLocation)
{
	if ( WallMeshes.IsEmpty() )
	{
		UE_LOG(RoomsManagerLog, Error,
		       TEXT("%s : WallMeshes is empty."), __FUNCTIONW__);
		return;
	}

	// TODO: set RoomActor's direction neighbors (Needed for decoration)
	// FRoomShapeDetails RoomDetails = GRoomShapeDetails[LevelRoom.RoomShape];
	// for ( int i = 0; i < RoomDetails.OccupiedTilesAmount; ++i )
	// {
	// 	const FIntPoint& RelativeRoomLoc = RoomDetails.OccupiedTilesLocations[i];
	// 	for ( int j = 0; j < 4; ++j )
	// 	{
	// 		FIntPoint         NeighborLocation = LevelLocation + RelativeRoomLoc + KIntPointByDirection[j];
	// 		const FLevelRoom& Neighbor         = LevelMap.GetSafe(NeighborLocation);
	// 		if ( !Neighbor.bOccupied )
	// 		{
	// 			continue;
	// 		}
	//
	// 		if ( Neighbor.Id == LevelRoom.Id )
	// 		{
	// 			continue;
	// 		}
	// 	}
	// }

	TArray<UWallComponent*> Walls;
	RoomActor.GetComponents<UWallComponent>(Walls);
	if ( Walls.IsEmpty() )
	{
		UE_LOG(RoomsManagerLog, Error,
		       TEXT("%s : %s doesn't contain any UWallComponent."), __FUNCTIONW__,
		       *RoomActor.GetName());
		return;
	}

	ChangeRoomWalls(Walls, LevelMap, LevelRoom, LevelLocation);
}

void ARoomsManager::SpawnDecoration(ARoomBase& RoomActor, const FLevelRoom& LevelRoom)
{
	if ( !IsValid(RoomDecorationsDataTable) )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : RoomDecorationDataTable is invalid."), __FUNCTIONW__);
		return;
	}

	TArray<FName> RowNames = RoomDecorationsDataTable->GetRowNames();
	if ( RowNames.IsEmpty() )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : RoomDecorationDataTable is empty."), __FUNCTIONW__);
		return;
	}

	FRoomDecorationInfo* DecorationInfo          = nullptr;
	bool                 FoundSuitableDecoration = false;

	for ( int Attempts = 0; Attempts < 10 && !FoundSuitableDecoration; ++Attempts )
	{
		int RandIndex = FMath::RandRange(0, RowNames.Num() - 1);

		DecorationInfo = RoomDecorationsDataTable->
				FindRow<FRoomDecorationInfo>(RowNames[RandIndex], __FUNCTIONW__);

		if ( !DecorationInfo )
		{
			UE_LOG(LogTemp, Error, TEXT("%s : FindRow returned nullptr."), __FUNCTIONW__);
			continue;
		}

		if ( DecorationInfo->SuitableRoomShape != LevelRoom.RoomShape )
		{
			continue;
		}

		if ( !IsValid(DecorationInfo->DecorationClass) )
		{
			UE_LOG(LogTemp, Error, TEXT("%s : DecorationClass of %s is invalid."), __FUNCTIONW__,
			       *RowNames[RandIndex].GetPlainNameString());
			continue;
		}

		FoundSuitableDecoration = true;
	}

	if ( !FoundSuitableDecoration || !DecorationInfo )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : Doesn't found any suitable decoration for room of type[%d]."), __FUNCTIONW__,
		       static_cast<int>(LevelRoom.RoomType));
		return;
	}

	UWorld* World = GetWorld();
	if ( !IsValid(World) )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : World is invalid."), __FUNCTIONW__);
		return;
	}

	ARoomDecorationBase* RoomDecoration = World->SpawnActor<ARoomDecorationBase>(
			DecorationInfo->DecorationClass, RoomActor.GetActorLocation(), FRotator::ZeroRotator);

	if ( !IsValid(RoomDecoration) )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : Newly spawned RoomDecoration is invalid."), __FUNCTIONW__);
		return;
	}

	RoomActor.SetDecoration(RoomDecoration);
	
	// do other stuff
}

void ARoomsManager::ChangeRoomWalls(TArray<UWallComponent*>& Walls, const FLevelMap&     LevelMap,
                                    const FLevelRoom&        LevelRoom, const FIntPoint& LevelLocation)
{
	for ( auto Wall : Walls )
	{
		if ( !IsValid(Wall) )
		{
			UE_LOG(RoomsManagerLog, Error, TEXT("%s : WallComponent is invalid."), __FUNCTIONW__);
			continue;
		}

		FIntPoint         PivotLocation = Wall->GetPivotPoint() + LevelLocation;
		const FIntPoint&  Direction     = Wall->GetDirection();
		const FLevelRoom& Neighbor      = LevelMap.GetSafe(PivotLocation + Direction);

		const FIntPoint AbsDir = {FMath::Abs(Direction.X), FMath::Abs(Direction.Y)};
		if ( !WallMeshes.Contains(AbsDir) )
		{
			UE_LOG(RoomsManagerLog, Error, TEXT("ARoomManager: Wall meshes does not contains needed meshes (dir: %s)"),
			       *AbsDir.ToString());
			return;
		}

		const FRoomWallMeshes& Meshes = WallMeshes[AbsDir];
		if ( Neighbor.bOccupied && Neighbor.Id != LevelRoom.Id )
		{
			Wall->SetStaticMesh(Wall->IsShort() ? Meshes.ShortDoorway : Meshes.DefaultDoorway);
		}
		else
		{
			Wall->SetStaticMesh(Wall->IsShort() ? Meshes.ShortWall : Meshes.DefaultWall);
		}
	}
}

void ARoomsManager::PlaceDoors(const FLevelMap& LevelMap)
{
	/** Going through whole LevelMap in a chess pattern
	 * to prevent placing duplicate doors. */
	for ( int y = 0; y < LevelMap.GetHeight(); ++y )
	{
		for ( int x = y % 2; x < LevelMap.GetWidth(); x += 2 )
		{
			// UE_LOG(RoomsManagerLog, Display, TEXT(" LOC: %02d %02d"), x, y);
			FIntPoint         CurrentLocation{x, y};
			const FLevelRoom& CurrentRoom = LevelMap.Get(CurrentLocation);
			if ( !CurrentRoom.bOccupied )
			{
				continue;
			}

			ARoomBase* CurrentRoomActor = CurrentRoom.RoomActorRef;
			if ( !IsValid(CurrentRoomActor) )
			{
				UE_LOG(RoomsManagerLog, Warning,
				       TEXT("%s : RoomActor at [%d,%d] is invalid, but location is occupied."), __FUNCTIONW__,
				       CurrentLocation.X, CurrentLocation.Y);
				continue;
			}

			for ( auto Direction : KIntPointByDirection )
			{
				FIntPoint NeighborLocation = CurrentLocation + Direction;
				if ( !LevelMap.IsOccupiedSafe(NeighborLocation) )
				{
					continue;
				}

				const FLevelRoom& NeighborRoom = LevelMap.Get(NeighborLocation);
				if ( CurrentRoom.Id == NeighborRoom.Id )
				{
					continue;
				}

				ARoomBase* NeighborRoomActor = CurrentRoom.RoomActorRef;
				if ( !IsValid(NeighborRoomActor) )
				{
					UE_LOG(RoomsManagerLog, Warning,
					       TEXT("%s : RoomActor at [%d,%d] is invalid, but location is occupied."),
					       __FUNCTIONW__, NeighborLocation.X, NeighborLocation.Y);
					continue;
				}


				ERoomType DoorType = ERoomType::Default;
				if ( NeighborRoom.RoomType != ERoomType::Default )
				{
					DoorType = NeighborRoom.RoomType;
				}
				else if ( CurrentRoom.RoomType != ERoomType::Default )
				{
					DoorType = CurrentRoom.RoomType;
				}

				FVector DoorLocation = MapToWorldRoomLocation(CurrentLocation) + FVector(Direction.X, Direction.Y, 0) *
				                       (RoomsLocationDelta / 2);

				if ( !DoorMeshes.Contains(DoorType) )
				{
					UE_LOG(RoomsManagerLog, Error,
					       TEXT("%s : DoorMeshes doesn't contain DoorType[%d]."), __FUNCTIONW__, DoorType);
					continue;
				}

				ADoor* Door = GetWorld()->SpawnActor<ADoor>(DoorClass);
				Door->SetType(DoorType);

				Door->SetDoorMesh(DoorMeshes[DoorType]);
				if ( !IsValid(Door) )
				{
					UE_LOG(RoomsManagerLog, Error, TEXT("%s : Spawned ADoor is invalid."), __FUNCTIONW__);
					continue;
				}

				Door->SetActorLocation(DoorLocation);

				if ( Direction.X == 0 )
				{
					Door->SetActorRotation({0, 90, 0});
				}


				CurrentRoomActor->OnRoomCleared.AddUniqueDynamic(Door, &ADoor::OnRoomCleared);
				NeighborRoomActor->OnRoomCleared.AddUniqueDynamic(Door, &ADoor::OnRoomCleared);

				CurrentRoomActor->OnPlayerEnteredRoom.AddUniqueDynamic(Door, &ADoor::OnPlayerEnteredRoom);
				NeighborRoomActor->OnPlayerEnteredRoom.AddUniqueDynamic(Door, &ADoor::OnPlayerEnteredRoom);


				UE_LOG(RoomsManagerLog, Display,
				       TEXT("%s : Spawned door from id[%02d] to id[%02d]"), __FUNCTIONW__, CurrentRoom.Id,
				       NeighborRoom.Id);
			}
		}
	}
}

FVector ARoomsManager::MapToWorldRoomLocation(const FIntPoint& RoomLocation) const
{
	return {
			RoomsLocationDelta.X * (RoomLocation.X - CentralRoomLocation.X),
			RoomsLocationDelta.Y * (RoomLocation.Y - CentralRoomLocation.Y),
			0
	};
}
