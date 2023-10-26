// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomsManager.h"

#include "Doors/Door.h"
#include "Level/LevelMap.h"
#include "Rooms/WallComponent.h"

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

void ARoomsManager::OnLevelGenerationCompleted(const FLevelMap& LevelMap, const TArray<FIntPoint>& RoomLocations)
{
	CentralRoomLocation = {LevelMap.GetWidth() / 2, LevelMap.GetHeight() / 2};
	for ( int i = 0; i < RoomLocations.Num(); ++i )
	{
		FIntPoint RoomLocation;
		RoomLocation = RoomLocations[i];

		const FLevelRoom& Room = LevelMap.Get(RoomLocation);
		if ( !Room.bOriginTile )
		{
			continue;
		}

		FVector RoomSpawnLocation = MapToWorldRoomLocation(RoomLocation);

		ARoomBase* RoomActor = SpawnRoom(Room, RoomSpawnLocation);
		ConstructRoom(RoomActor, LevelMap, Room, RoomLocation);
	}
	PlaceDoors(LevelMap);
}

ARoomBase* ARoomsManager::SpawnRoom(const FLevelRoom& LevelRoom, const FVector& WorldLocation)
{
	if ( RoomClasses.IsEmpty() || !RoomClasses.Contains(LevelRoom.RoomShape) )
	{
		return nullptr;
	}

	ARoomBase* RoomActor = GetWorld()->SpawnActor<ARoomBase>(RoomClasses[LevelRoom.RoomShape], WorldLocation,
	                                                         FRotator::ZeroRotator);

	return RoomActor;
}

void ARoomsManager::ConstructRoom(ARoomBase*       RoomActor, const FLevelMap& LevelMap, const FLevelRoom& LevelRoom,
                                  const FIntPoint& LevelLocation)
{
	if ( !IsValid(RoomActor) )
	{
		UE_LOG(RoomsManagerLog, Error, TEXT("RoomManager::ChangeRoomWalls : RoomActor is invalid."));
		return;
	}

	if ( WallMeshes.IsEmpty() )
	{
		UE_LOG(RoomsManagerLog, Error,
		       TEXT("RoomManager::ChangeRoomWalls : WallMeshes is empty, please set wall meshes."));
		return;
	}

	TArray<UWallComponent*> Walls;
	RoomActor->GetComponents<UWallComponent>(Walls);
	if ( Walls.IsEmpty() )
	{
		UE_LOG(RoomsManagerLog, Error,
		       TEXT("RoomManager::ChangeRoomWalls : RoomActor (%s) doesn't contain any UWallComponent."),
		       *RoomActor->GetName());
		return;
	}

	ChangeRoomWalls(Walls, LevelMap, LevelRoom, LevelLocation);
}

void ARoomsManager::ChangeRoomWalls(TArray<UWallComponent*>& Walls, const FLevelMap&     LevelMap,
                                    const FLevelRoom&        LevelRoom, const FIntPoint& LevelLocation)
{
	for ( auto Wall : Walls )
	{
		if ( !IsValid(Wall) )
		{
			UE_LOG(RoomsManagerLog, Error, TEXT("RoomManager::ChangeRoomWalls : WallComponent is invalid."));
			continue;
		}

		FIntPoint         PivotLocation = Wall->GetPivotPoint() + LevelLocation;
		const FIntPoint&  Direction     = Wall->GetDirection();
		const FLevelRoom& Neighbor      = LevelMap.GetSafe(PivotLocation + Direction);

		const FIntPoint AbsDir = {FMath::Abs(Direction.X), FMath::Abs(Direction.Y)};
		if ( !WallMeshes.Contains(AbsDir) )
		{
			UE_LOG(RoomsManagerLog, Error, TEXT("RoomManager: Wall meshes does not contains needed meshes (dir: %s)"),
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
			UE_LOG(RoomsManagerLog, Display, TEXT(" LOC: %02d %02d"), x, y);
			FIntPoint         CurrentLocation{x, y};
			const FLevelRoom& CurrentRoom = LevelMap.Get(CurrentLocation);
			if ( !CurrentRoom.bOccupied )
			{
				continue;
			}

			for ( auto Direction : KIntPointByDirection )
			{
				FIntPoint NeighborLocation = CurrentLocation + Direction;
				// UE_LOG(RoomsManagerLog, Display, TEXT(" NEIGHBOR_LOC: %02d %02d"), NeighborLocation.X, NeighborLocation.Y);
				if ( !LevelMap.IsOccupiedSafe(NeighborLocation) )
				{
					continue;
				}

				const FLevelRoom& NeighborRoom = LevelMap.Get(NeighborLocation);
				if ( CurrentRoom.Id == NeighborRoom.Id )
				{
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

				FVector DoorLocation = MapToWorldRoomLocation(CurrentLocation) + FVector(Direction.X, Direction.Y, 0) * (RoomsLocationDelta / 2);
				
				ADoor* Door = GetWorld()->SpawnActor<ADoor>(DoorClass);
				// Door->SetType();
				if ( !DoorMeshes.Contains(DoorType) )
				{
					UE_LOG(RoomsManagerLog, Error,
					       TEXT("RoomsManager::PlaceDoors : DoorMeshes doesn't contain DoorType[%d]."), DoorType);
					continue;
				}

				Door->SetDoorMesh(DoorMeshes[DoorType]);
				if ( !IsValid(Door) )
				{
					UE_LOG(RoomsManagerLog, Error, TEXT("RoomsManager::PlaceDoors : Spawned ADoor isn't valid."));
					continue;
				}

				Door->SetActorLocation(DoorLocation);

				if ( Direction.X == 0 )
				{
					Door->SetActorRotation({0, 90, 0});
				}
				UE_LOG(RoomsManagerLog, Display, TEXT(" SPAWNED DOOR FROM id[%02d] TO id[%02d]"), CurrentRoom.Id, NeighborRoom.Id);
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
