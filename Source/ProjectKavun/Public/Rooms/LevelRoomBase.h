// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelRoomBase.generated.h"

UCLASS()
class PROJECTKAVUN_API ALevelRoomBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ALevelRoomBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
};
