// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomsManager.h"

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
	const FIntPoint CentralRoomLocation = {LevelMap.GetWidth() / 2, LevelMap.GetHeight() / 2};
	for ( int i = 0; i < RoomLocations.Num(); ++i )
	{
		FIntPoint RoomLocation;
		RoomLocation = RoomLocations[i];

		const FLevelRoom& Room = LevelMap.Get(RoomLocation);
		if ( !Room.bOriginTile )
		{
			continue;
		}

		FVector SpawnLocation = {
				RoomsLocationDelta.X * (RoomLocation.X - CentralRoomLocation.X),
				RoomsLocationDelta.Y * (RoomLocation.Y - CentralRoomLocation.Y),
				0
		};

		ARoomBase* RoomActor = SpawnRoom(Room, SpawnLocation);
		ChangeRoomWalls(RoomActor, LevelMap, Room, RoomLocation);
	}
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

void ARoomsManager::ChangeRoomWalls(ARoomBase*       RoomActor, const FLevelMap& LevelMap, const FLevelRoom& LevelRoom,
                                    const FIntPoint& LevelLocation)
{
	if(!IsValid(RoomActor))
	{
		UE_LOG(RoomsManagerLog, Error, TEXT("RoomManager::ChangeRoomWalls : RoomActor is invalid."));
		return;
	}
	
	if ( WallMeshes.IsEmpty() )
	{
		UE_LOG(RoomsManagerLog, Error, TEXT("RoomManager::ChangeRoomWalls : WallMeshes is empty, please set wall meshes."));
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

	for ( auto Wall : Walls )
	{
		if(!IsValid(Wall))
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
