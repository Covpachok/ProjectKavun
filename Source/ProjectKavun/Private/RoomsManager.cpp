// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomsManager.h"

#include "Level/LevelMap.h"

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

void ARoomsManager::OnLevelGenerationCompleted(const FLevelMap& LevelMap, const TArray<FIntVector2>& RoomLocations)
{
	const FIntVector2 CentralRoomLocation = {LevelMap.GetWidth() / 2, LevelMap.GetHeight() / 2};
	for ( int i = 0; i < RoomLocations.Num(); ++i )
    	{
    		FIntVector2 RoomLocation;
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
    
    		SpawnRoom(LevelMap, SpawnLocation, Room.RoomShape, Room.RoomType);
    	}
}

void ARoomsManager::SpawnRoom(const FLevelMap& LevelMap, const FVector& Location, ERoomShape Shape, ERoomType Type)
{
	if(RoomsClasses.IsEmpty() || !RoomsClasses.Contains(Shape))
	{
		return;
	}
	
	GetWorld()->SpawnActor<AActor>(RoomsClasses[Shape], Location, FRotator::ZeroRotator);
}

