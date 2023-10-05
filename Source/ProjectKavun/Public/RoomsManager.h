// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rooms/LevelRoomBase.h"
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

	void SpawnRoom(const FVector &Location, ERoomShape Shape, ERoomType Type);

private:
	UPROPERTY(EditAnywhere)
	TMap<ERoomShape, TSubclassOf<AActor>> RoomsClasses;
};
