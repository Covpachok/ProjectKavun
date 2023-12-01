// Fill out your copyright notice in the Description page of Project Settings.


#include "Doors/Door.h"

#include "Components/BoxComponent.h"
#include "Aliases.h"
#include "Characters/PlayerCharacter.h"

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
	
	bReplicates = true;
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

void ADoor::Open()
{
	DoorMesh->SetHiddenInGame(true);
	DoorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADoor::Close()
{
	DoorMesh->SetHiddenInGame(false);
	DoorMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ADoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32                OtherBodyIndex, bool         bFromSweep, const FHitResult& SweepResult)
{
	if ( !IsValid(OtherActor) )
	{
		return;
	}

	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if ( !IsValid(Player) )
	{
		return;
	}

	if ( bClosed )
	{
		return;
	}

	if ( KeysNeeded )
	{
		/* TODO: Some kind of timer that will check if player still trying to use the keys
			Also this thing kind of shares functionality with chests */
		Player->SubtractKeys(1);
		--KeysNeeded;
		if ( KeysNeeded == 0 )
		{
			Open();
		}
	}
	else
	{
		Open();
	}
}

void ADoor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                         int32                OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow,
	                                 FString::Printf(TEXT("Door end overlap with: %s"), *OtherActor->GetName()));
}

void ADoor::OnRoomCleared()
{
	bClosed = false;
	if ( !KeysNeeded )
	{
		Open();
	}
}

void ADoor::OnPlayerEnteredRoom(APlayerCharacter* Player, bool bRoomClear)
{
	UE_LOG(LogTemp, Warning, TEXT("BABABOEY"));
	bClosed = !bRoomClear;
	if ( !bClosed && !KeysNeeded )
	{
		Open();
	}
}
