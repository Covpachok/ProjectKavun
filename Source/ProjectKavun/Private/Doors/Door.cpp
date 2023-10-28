// Fill out your copyright notice in the Description page of Project Settings.


#include "Doors/Door.h"

#include "Components/BoxComponent.h"
#include "Aliases.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;

	bClosed    = false;
	KeysNeeded = 0;

	DoorFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrameMesh"));
	SetRootComponent(DoorFrameMesh);

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(RootComponent);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);

	BoxCollision->SetGenerateOverlapEvents(true);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxCollision->SetCollisionObjectType(ECC_WorldDynamic);
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECC_PLAYER_CHARACTER, ECR_Overlap);

	BoxCollision->SetRelativeLocation({0, 0, 150});
	BoxCollision->SetBoxExtent({100, 100, 150});

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnOverlapEnd);
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADoor::SetDoorMesh(UStaticMesh* NewMesh)
{
	DoorMesh->SetStaticMesh(NewMesh);
}

void ADoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32                OtherBodyIndex, bool         bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green,
	                                 FString::Printf(TEXT("Door overlapped with: %s"), *OtherActor->GetName()));

	if ( KeysNeeded && bClosed )
	{
		// Player->DecreaseKeysCount();
		// --KeysNeeded;
		// if(KeysNeeded == 0)
		// bClosed = false;
	}

	if ( !bClosed )
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Purple,
		                                 FString::Printf(TEXT("Door opened!")));
		DoorMesh->SetHiddenInGame(true);
		DoorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ADoor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                         int32                OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow,
	                                 FString::Printf(TEXT("Door end overlap with: %s"), *OtherActor->GetName()));

	DoorMesh->SetHiddenInGame(false);
	DoorMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ADoor::OnRoomCleared()
{
	bClosed = false;
}

void ADoor::OnPlayerEnteredRoom(bool bRoomClear)
{
	bClosed = true;
}
