// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rooms/RoomBase.h"
#include "LevelGenerator.h"
#include "RoomsManager.generated.h"

UCLASS()
class PROJECTKAVUN_API ARoomsManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ARoomsManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void OnLevelGenerationCompleted(const FLevelMap &LevelMap, const TArray<FIntVector2> &RoomLocations);

private:
	void SpawnRoom(const FLevelMap &LevelMap, const FVector &Location, ERoomShape Shape, ERoomType Type);

private:
	UPROPERTY(EditAnywhere)
	FVector RoomsLocationDelta;

	UPROPERTY(EditAnywhere)
	TMap<ERoomShape, TSubclassOf<AActor>> RoomsClasses;
};
