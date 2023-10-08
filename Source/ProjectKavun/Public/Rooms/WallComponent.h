// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "WallComponent.generated.h"

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class PROJECTKAVUN_API UWallComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	FIntPoint GetDirection() const { return Direction; };
	
	UFUNCTION(BlueprintCallable)
	FIntPoint GetPivotPoint() const { return PivotPoint; };
	
	UFUNCTION(BlueprintCallable)
	bool IsShort() const { return bShortWall; };

private:
	UPROPERTY(EditAnywhere)
	FIntPoint Direction;

	UPROPERTY(EditAnywhere)
	FIntPoint PivotPoint;
	
	UPROPERTY(EditAnywhere)
	bool bShortWall;
};
