// Fill out your copyright notice in the Description page of Project Settings.


#include "Rooms/RoomBase.h"

#include "Rooms/WallComponent.h"

// Sets default values
ARoomBase::ARoomBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerDetector = CreateDefaultSubobject<UBoxComponent>(TEXT("PlayerDetector"));
	PlayerDetector->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ARoomBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARoomBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARoomBase::ConstructRoom()
{
}