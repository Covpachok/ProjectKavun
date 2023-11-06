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
			UE_LOG(RoomsManagerLog, Error, TEXT("ARoomsManager::OnLevelGenerationCompleted : RoomActor is invalid"));
			continue;
		}
		Room.RoomActorRef = RoomActor;
		ConstructRoom(RoomActor, LevelMap, Room, RoomLocation);
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
		UE_LOG(LevelGeneratorLog, Display, TEXT("ARoomsManager::SpawnRoom : Special room spawned"))
	}
	else
	{
		RoomActor = GetWorld()->SpawnActor<ARoomBase>(RoomClassesByShape[LevelRoom.RoomShape], WorldLocation,
		                                              FRotator::ZeroRotator);
	}
	
	RoomActor->SetShape(LevelRoom.RoomShape);
	RoomActor->SetType(LevelRoom.RoomType);

	return RoomActor;
}

void ARoomsManager::ConstructRoom(ARoomBase*       RoomActor, const FLevelMap& LevelMap, const FLevelRoom& LevelRoom,
                                  const FIntPoint& LevelLocation)
{
	if ( !IsValid(RoomActor) )
	{
		UE_LOG(RoomsManagerLog, Error, TEXT("ARoomManager::ChangeRoomWalls : RoomActor is invalid."));
		return;
	}

	if ( WallMeshes.IsEmpty() )
	{
		UE_LOG(RoomsManagerLog, Error,
		       TEXT("ARoomManager::ChangeRoomWalls : WallMeshes is empty, please set wall meshes."));
		return;
	}

	TArray<UWallComponent*> Walls;
	RoomActor->GetComponents<UWallComponent>(Walls);
	if ( Walls.IsEmpty() )
	{
		UE_LOG(RoomsManagerLog, Error,
		       TEXT("ARoomManager::ChangeRoomWalls : RoomActor (%s) doesn't contain any UWallComponent."),
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
			UE_LOG(RoomsManagerLog, Error, TEXT("ARoomManager::ChangeRoomWalls : WallComponent is invalid."));
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
				       TEXT("ARoomsManager::PlaceDoors : RoomActor at [%d,%d] is invalid, but location is occupied."),
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
					       TEXT("ARoomsManager::PlaceDoors : RoomActor at [%d,%d] is invalid, but location is occupied."
					       ),
					       NeighborLocation.X, NeighborLocation.Y);
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
					       TEXT("ARoomsManager::PlaceDoors : DoorMeshes doesn't contain DoorType[%d]."), DoorType);
					continue;
				}

				ADoor* Door = GetWorld()->SpawnActor<ADoor>(DoorClass);
				Door->SetType(DoorType);
				
				Door->SetDoorMesh(DoorMeshes[DoorType]);
				if ( !IsValid(Door) )
				{
					UE_LOG(RoomsManagerLog, Error, TEXT("ARoomsManager::PlaceDoors : Spawned ADoor isn't valid."));
					continue;
				}

				Door->SetActorLocation(DoorLocation);

				if ( Direction.X == 0 )
				{
					Door->SetActorRotation({0, 90, 0});
				}


				CurrentRoomActor->OnRoomCleared.AddDynamic(Door, &ADoor::OnRoomCleared);
				NeighborRoomActor->OnRoomCleared.AddDynamic(Door, &ADoor::OnRoomCleared);

				CurrentRoomActor->OnPlayerEnteredRoom.AddDynamic(Door, &ADoor::OnPlayerEnteredRoom);
				NeighborRoomActor->OnPlayerEnteredRoom.AddDynamic(Door, &ADoor::OnPlayerEnteredRoom);


				UE_LOG(RoomsManagerLog, Display,
				       TEXT("ARoomsManager::PlaceDoors : Spawned door from id[%02d] to id[%02d]"), CurrentRoom.Id,
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
