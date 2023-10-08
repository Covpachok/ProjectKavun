// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomsManager.h"

#include "Level/LevelMap.h"
#include "Rooms/WallComponent.h"

// Sets default values
ARoomsManager::ARoomsManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARoomsManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
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
	if ( WallMeshes.IsEmpty() )
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Purple,
		                                 FString::Printf(TEXT("RoomManager: Please set wall meshes")));
		return;
	}

	TArray<UWallComponent*> Walls;
	RoomActor->GetComponents<UWallComponent>(Walls);
	if ( Walls.IsEmpty() )
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Purple, FString::Printf(TEXT("RoomManager: Walls not found")));
		return;
	}

	for ( auto Wall : Walls )
	{
		FIntPoint         PivotLocation = Wall->GetPivotPoint() + LevelLocation;
		const FIntPoint&  Direction     = Wall->GetDirection();
		const FLevelRoom& Neighbor      = LevelMap.GetSafe(PivotLocation + Direction);

		const FIntPoint AbsDir = {FMath::Abs(Direction.X), FMath::Abs(Direction.Y)};
		if ( !WallMeshes.Contains(AbsDir) )
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Purple,
			                                 FString::Printf(
					                                 TEXT("RoomManager: Wall meshes does not contains needed meshes (%s)"), *AbsDir.ToString()));
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
