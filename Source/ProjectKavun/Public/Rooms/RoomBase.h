// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/RectLightComponent.h"
#include "GameFramework/Actor.h"
#include "RoomBase.generated.h"

class AKavunCamera;
class UCameraAnchor;
class APlayerCharacter;
class UPlayerDetector;
class UCameraBlockerComponent;
class UWallComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoomClearedSignature);

UENUM(BlueprintType)
enum class ERoomType : uint8
{
	Default,
	Boss,
	Treasure,
	Secret,
	Shop
	// ETC
};

UENUM(BlueprintType)
enum class ERoomShape : uint8
{
	/* [0] */
	Square,

	/* [0]
	 * [0][1] */
	BigL_0,

	/*    [1]
	 * [0][1] */
	BigL_1,

	/* [0][1]
	 * [0]   */
	BigL_2,

	/* [0][1]
	 *    [1] */
	BigL_3,

	/* [0 1]
	 * [0 1] */
	BigSquare,

	/* [0]
	 * [0] */
	BigStraight_0,

	/* [0][1] */
	BigStraight_1,

	/* [-] */
	Narrow_0,

	/* [|] */
	Narrow_1,

	/* []
	 * [] */
	LongNarrow_0,

	/* [][] */
	LongNarrow_1,

	Amount UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ERoomShapeType : uint8
{
	Default,
	Big,
	Narrow
};

struct FRoomShapeDetails
{
	ERoomShapeType ShapeType;

	int       OccupiedTilesAmount;
	FIntPoint OccupiedTilesLocations[4];

	bool bLeftRightAccessible = true;
	bool bUpDownAccessible    = true;
};

struct FRoomPieceInfo
{
	FIntPoint RelativeLocation;
	bool      OccupiedNeighbors[4];
};

const TMap<ERoomShape, FRoomShapeDetails> GRoomShapeDetails
{
		{{ERoomShape::Square}, {ERoomShapeType::Default, 1, {{0, 0}}}},
		{{ERoomShape::BigL_0}, {ERoomShapeType::Big, 3, {{0, 0}, {0, 1}, {1, 1}}}},
		{{ERoomShape::BigL_1}, {ERoomShapeType::Big, 3, {{0, 0}, {0, 1}, {-1, 1}}}},
		{{ERoomShape::BigL_2}, {ERoomShapeType::Big, 3, {{0, 0}, {1, 0}, {0, 1}}}},
		{{ERoomShape::BigL_3}, {ERoomShapeType::Big, 3, {{0, 0}, {1, 0}, {1, 1}}}},
		{{ERoomShape::BigSquare}, {ERoomShapeType::Big, 4, {{0, 0}, {1, 0}, {0, 1}, {1, 1}}}},
		{{ERoomShape::BigStraight_0}, {ERoomShapeType::Big, 2, {{0, 0}, {0, 1}}}},
		{{ERoomShape::BigStraight_1}, {ERoomShapeType::Big, 2, {{0, 0}, {1, 0}}}},
		{{ERoomShape::Narrow_0}, {ERoomShapeType::Narrow, 1, {{0, 0}}, false}},
		{{ERoomShape::Narrow_1}, {ERoomShapeType::Narrow, 1, {{0, 0}}, true, false}},
		{{ERoomShape::LongNarrow_0}, {ERoomShapeType::Narrow, 2, {{0, 0}, {0, 1}}, false}},
		{{ERoomShape::LongNarrow_1}, {ERoomShapeType::Narrow, 2, {{0, 0}, {1, 0}}, true, false}},
};

UCLASS(Abstract)
class PROJECTKAVUN_API ARoomBase : public AActor
{
	GENERATED_BODY()

public:
	FOnRoomClearedSignature OnRoomCleared;

public:
	ARoomBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void ConstructRoom(UDataTable* RoomDecorationsTable);

	void SetDirectionOccupied();

	UFUNCTION(BlueprintNativeEvent, Category="Room")
	void PlayerEnteredRoom(APlayerCharacter* Player);

	UFUNCTION(BlueprintNativeEvent, Category="Room")
	void PlayerExitedRoom(APlayerCharacter* Player);

	UFUNCTION(BlueprintNativeEvent, Category="Room")
	void OnConstructionCompleted();

	UFUNCTION(BlueprintCallable, Category="Room")
	void SetType(ERoomType NewType) { Type = NewType; }

	UFUNCTION(BlueprintCallable, Category="Room")
	void SetShape(ERoomShape NewShape) { Shape = NewShape; }

	/* Other */

	UFUNCTION(BlueprintCallable, Category="Room")
	void SetLightsVisibility(bool bVisible);

private:
	void TeleportCamera(AKavunCamera *Camera);

protected:
	// /* Walls */
	//
	// UPROPERTY(EditAnywhere, Category="Components")
	// TObjectPtr<UWallComponent> Wall1;
	//
	// UPROPERTY(EditAnywhere, Category="Components")
	// TObjectPtr<UWallComponent> Wall2;
	//
	// UPROPERTY(EditAnywhere, Category="Components")
	// TObjectPtr<UWallComponent> Wall3;
	//
	// UPROPERTY(EditAnywhere, Category="Components")
	// TObjectPtr<UWallComponent> Wall4;
	//
	// /* Other Components */
	//
	// UPROPERTY(EditAnywhere, Category="Components")
	// TObjectPtr<UStaticMeshComponent> Floor1;
	//
	// UPROPERTY(EditAnywhere, Category="Components")
	// TObjectPtr<URectLightComponent> Light1;
	//
	// UPROPERTY(EditAnywhere, Category="Components")
	// TObjectPtr<UPlayerDetector> PlayerDetector1;
	//
	// UPROPERTY(EditAnywhere, Category="Components")
	// TObjectPtr<UCameraBlockerComponent> CameraBlocker1;
	//
	// UPROPERTY(EditAnywhere, Category="Components")
	// TObjectPtr<UCameraBlockerComponent> CameraBlocker2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	ERoomShape Shape;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	ERoomType Type;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	bool bRoomClear;


	/* Component refs */

	TArray<TObjectPtr<URectLightComponent>> Lights;
	TArray<TObjectPtr<UWallComponent>>      Walls;
	TArray<TObjectPtr<UCameraAnchor>>     CameraAnchors;
};
