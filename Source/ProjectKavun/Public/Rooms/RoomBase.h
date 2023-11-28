// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utilities.h"
#include "Components/RectLightComponent.h"
#include "GameFramework/Actor.h"
#include "Rooms/RoomDetails.h"
#include "RoomBase.generated.h"

class AEnemyCharacter;
class ARoomDecorationBase;
class UBoxComponent;
class AKavunCamera;
class UCameraAnchor;
class APlayerCharacter;
class UDetectorComponent;
class UCameraBlockerComponent;
class UWallComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoomClearedDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerEnteredDelegate, APlayerCharacter*, Player, bool, bRoomClear);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerExitedDelegate, APlayerCharacter*, Player);


UCLASS(Abstract)
class PROJECTKAVUN_API ARoomBase : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Room")
	FOnRoomClearedDelegate OnRoomCleared;
	UPROPERTY(BlueprintAssignable, Category = "Room")
	FOnPlayerEnteredDelegate OnPlayerEnteredRoom;
	UPROPERTY(BlueprintAssignable, Category = "Room")
	FOnPlayerExitedDelegate OnPlayerExitedRoom;

public:
	ARoomBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void Construct(UDataTable* RoomDecorationsTable);

	void SetDirectionOccupied(const FIntPoint& PieceRelativeLocation, EDirections Direction);


	UFUNCTION(BlueprintNativeEvent, Category="Room")
	void OnPlayerEntered(APlayerCharacter* Player);

	UFUNCTION(BlueprintNativeEvent, Category="Room")
	void OnPlayerExited(APlayerCharacter* Player);

	UFUNCTION(BlueprintNativeEvent, Category="Room")
	void OnConstructionCompleted();

	void SetDecoration(ARoomDecorationBase* RoomDecoration) { Decoration = RoomDecoration; }

	void SetType(ERoomType NewType) { Type = NewType; }


	UFUNCTION(BlueprintGetter, Category = "Room")
	ERoomShape GetShape() const { return Shape; }

	UFUNCTION(BlueprintGetter, Category = "Room")
	ERoomType GetType() const { return Type; }

	UFUNCTION(BlueprintGetter, Category = "Room")
	bool IsRoomClear() const { return bRoomClear; }

private:
	void SetLightsVisibility(bool bVisible);
	void TeleportCamera(AKavunCamera* Camera);

	void InitComponentRefs();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintGetter = "GetShape")
	ERoomShape Shape;

	UPROPERTY(EditDefaultsOnly, BlueprintGetter = "GetType")
	ERoomType Type;

	UPROPERTY(EditDefaultsOnly, BlueprintGetter = "IsRoomClear")
	bool bRoomClear;

	/* */

	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<ARoomDecorationBase> Decoration;

	/* Component refs */

	TArray<TObjectPtr<URectLightComponent>> Lights;
	TArray<TObjectPtr<UWallComponent>>      Walls;
	TArray<TObjectPtr<UCameraAnchor>>       CameraAnchors;

	TSet<TObjectPtr<AActor>> ActorsInside;
};
