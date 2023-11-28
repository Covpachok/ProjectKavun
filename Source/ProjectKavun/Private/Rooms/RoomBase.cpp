// Fill out your copyright notice in the Description page of Project Settings.


#include "Rooms/RoomBase.h"

#include "KavunCamera.h"
#include "Utilities.h"
#include "Characters/PlayerCharacter.h"
#include "Components/DetectorComponent.h"
#include "Rooms/WallComponent.h"
#include "Rooms/RoomDecorationBase.h"
#include "Components/CameraAnchor.h"

ARoomBase::ARoomBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARoomBase::BeginPlay()
{
	Super::BeginPlay();

	InitComponentRefs();

	// const FRoomShapeDetails& ShapeDetails = GRoomShapeDetails[Shape];
	// for ( int i = 0; i < ShapeDetails.OccupiedTilesAmount; ++i )
	// {
	// 	PiecesInfo.Add(ShapeDetails.OccupiedTilesLocations[i]);
	// }
}

void ARoomBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARoomBase::Construct(UDataTable* RoomDecorationsTable)
{
}

void ARoomBase::SetDirectionOccupied(const FIntPoint& PieceRelativeLocation, EDirections Direction)
{
	// FRoomPieceInfo* PieceInfo = PiecesInfo.Find(PieceRelativeLocation);
	// if ( PieceInfo == nullptr )
	// {
		// UE_LOG(LogTemp, Error, TEXT("%s : PieceRelativeLocation(%s) not found on %s."), __FUNCTIONW__,
		       // *PieceRelativeLocation.ToString(), *GetName());
		// return;
	// }

	// PieceInfo->OccupiedNeighbors[Direction] = true;
}


void ARoomBase::SetLightsVisibility(bool bVisible)
{
	for ( auto& Light : Lights )
	{
		if ( !IsValid(Light) )
		{
			UE_LOG(LogTemp, Error, TEXT("%s : URectLightComponent is invalid."), __FUNCTIONW__);
			continue;
		}

		UE_LOG(LogTemp, Display, TEXT("%s : Lights turned on."), __FUNCTIONW__);
		Light->SetVisibility(bVisible);
	}
}

void ARoomBase::TeleportCamera(AKavunCamera* Camera)
{
	if ( !IsValid(Camera) )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : Camera is invalid."), __FUNCTIONW__);
		return;
	}

	FVector CameraLocation = Camera->GetActorLocation();
	CameraLocation.Z       = 0;

	FVector NewLocation;

	bool  bSuccess         = false;
	float SmallestDistance = 1000000;
	for ( auto& Anchor : CameraAnchors )
	{
		if ( !IsValid(Anchor) )
		{
			continue;
		}

		FVector AnchorLocation = Anchor->GetComponentLocation();
		AnchorLocation.Z       = 0;
		const float Distance   = FVector::Distance(CameraLocation, AnchorLocation);

		if ( Distance < SmallestDistance )
		{
			SmallestDistance = Distance;
			NewLocation      = AnchorLocation;
			bSuccess         = true;
		}
	}

	if ( !bSuccess )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : Unsuccessful."), __FUNCTIONW__);
		return;
	}

	Camera->Teleport(NewLocation);
}

void ARoomBase::InitComponentRefs()
{
	TArray<UDetectorComponent*> PlayerDetectors;
	GetComponents<UDetectorComponent>(PlayerDetectors);
	if ( PlayerDetectors.IsEmpty() )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : UPlayerDetectors not found on %s."), __FUNCTIONW__,
		       *GetName());
		return;
	}

	for ( const auto& Detector : PlayerDetectors )
	{
		if ( !IsValid(Detector) )
		{
			UE_LOG(LogTemp, Error, TEXT("%s : PlayerDetector is invalid on %s."), __FUNCTIONW__, *GetName());
			continue;
		}

		UE_LOG(LogTemp, Display, TEXT("%s : Detector functions registered."), __FUNCTIONW__);
		Detector->OnPlayerOverlapBegin.AddDynamic(this, &ARoomBase::OnPlayerEntered);
		Detector->OnPlayerOverlapEnd.AddDynamic(this, &ARoomBase::OnPlayerExited);
	}

	GetComponents<UWallComponent>(Walls);
	if ( Walls.IsEmpty() )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : UWallComponents not found on %s."), __FUNCTIONW__, *GetName());
		return;
	}

	GetComponents<URectLightComponent>(Lights);
	if ( Lights.IsEmpty() )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : URectLightComponents not found on %s."), __FUNCTIONW__, *GetName());
		return;
	}

	GetComponents<UCameraAnchor>(CameraAnchors);
	if ( CameraAnchors.IsEmpty() )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : CameraAnchors not found on %s."), __FUNCTIONW__, *GetName());
		return;
	}
}

void ARoomBase::OnPlayerEntered_Implementation(APlayerCharacter* Player)
{
	if ( !IsValid(Player) )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : Player is invalid."), __FUNCTIONW__);
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("%s : Player entered room %s."), __FUNCTIONW__, *GetName());

	TeleportCamera(Player->GetCameraActor());

	SetLightsVisibility(true);

	if(IsValid(Decoration))
	{
		Decoration->ActivateSpawners();
	}

	OnPlayerEnteredRoom.Broadcast(Player, bRoomClear);
}

void ARoomBase::OnPlayerExited_Implementation(APlayerCharacter* Player)
{
	UE_LOG(LogTemp, Display, TEXT("%s : Player exited room %s."), __FUNCTIONW__, *GetName());

	SetLightsVisibility(false);

	if(IsValid(Decoration))
	{
		Decoration->DeactivateSpawners();
	}
	
	OnPlayerExitedRoom.Broadcast(Player);
}

void ARoomBase::OnConstructionCompleted_Implementation()
{
}
