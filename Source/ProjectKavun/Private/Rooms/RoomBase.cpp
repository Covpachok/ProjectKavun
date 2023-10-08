// Fill out your copyright notice in the Description page of Project Settings.


#include "Rooms/RoomBase.h"

// Sets default values
ALevelRoomBase::ALevelRoomBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevelRoomBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelRoomBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

