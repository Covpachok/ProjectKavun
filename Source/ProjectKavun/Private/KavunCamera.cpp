// Fill out your copyright notice in the Description page of Project Settings.


#include "KavunCamera.h"

#include "Camera/CameraComponent.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

AKavunCamera::AKavunCamera()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCameraComponent()->FieldOfView = 90;
	GetCameraComponent()->AspectRatio = 16 / 9;
	GetCameraComponent()->SetWorldLocation({0, 0, 1500});
	GetCameraComponent()->SetWorldRotation({-90, 0, 0});
	GetCameraComponent()->SetAutoActivate(true);

	bFollowPosX = true;
	bFollowPosY = true;
	bFollowNegX = true;
	bFollowNegY = true;

	LastFollowLocation = GetActorLocation();
}

void AKavunCamera::BeginPlay()
{
	Super::BeginPlay();

	FollowCharacterRef = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if ( !IsValid(FollowCharacterRef) )
	{
		UE_LOG(LogTemp, Error, TEXT("AKavunCamera::BeginPlay : FollowCharacterRef is not valid."));
		return;
	}

	FollowCharacterRef->SetCameraActor(this);
}

void AKavunCamera::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	Follow(DeltaSeconds);
}

void AKavunCamera::ChangeFollow(bool bNewFollow)
{
	bFollowPosX = bNewFollow;
	bFollowNegX = bNewFollow;
	bFollowPosY = bNewFollow;
	bFollowNegY = bNewFollow;
}

void AKavunCamera::Follow(float Delta)
{
	if ( !IsValid(FollowCharacterRef) )
	{
		UE_LOG(LogTemp, Error, TEXT("AKavunCamera::Follow : FollowCharacterRef is not valid."));
		return;
	}

	const FVector CurrentLocation = GetActorLocation();
	FVector       FollowLocation  = FollowCharacterRef->GetActorLocation();

	const FVector DeltaLocation = FollowLocation - DeltaLocation;

	// UE_LOG(LogTemp, Display, TEXT("AKavunCamera::Follow : Delta %s\nx%d -x%d y%d -y%d"), *DeltaLocation.ToString(),
	// bFollowPosX, bFollowNegX, bFollowPosY, bFollowNegY);

	if ( (DeltaLocation.X < 0 && !bFollowNegX) ||
	     (DeltaLocation.X > 0 && !bFollowPosX) )
	{
		FollowLocation.X = LastFollowLocation.X;
	}

	if ( (DeltaLocation.Y < 0 && !bFollowNegY) ||
	     (DeltaLocation.Y > 0 && !bFollowPosY) )
	{
		FollowLocation.Y = LastFollowLocation.Y;
	}

	if ( (bFollowPosX && bFollowNegX) || (bFollowPosY && bFollowNegY) )
	{
		LastFollowLocation = FollowLocation;
	}

	FollowLocation.Z = CurrentLocation.Z;

	const FVector NewLocation = FMath::VInterpTo(CurrentLocation, FollowLocation, Delta, FollowSpeed);
	SetActorLocation(NewLocation);

	tempdebugvariablepleasedeleteme = DeltaLocation;
}
