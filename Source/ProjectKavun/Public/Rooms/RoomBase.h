// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "RoomBase.generated.h"

class UWallComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoomClearedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerEnteredRoomSignature, bool, bRoomClear);

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

	int         OccupiedTilesAmount;
	FIntPoint OccupiedTilesLocations[4];

	bool bLeftRightAccessible = true;
	bool bUpDownAccessible    = true;
};

USTRUCT(BlueprintType)
struct FRoomPivot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TMap<FIntPoint, UStaticMeshComponent*> Walls;

	UPROPERTY(EditAnywhere)
	TMap<FIntPoint, UStaticMeshComponent*> Doors;
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
	FOnPlayerEnteredRoomSignature OnPlayerEnteredRoom;

public:
	ARoomBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void ConstructRoom();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	ERoomShape Shape;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	ERoomType Type;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	UBoxComponent *PlayerDetector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	bool bRoomClear;
};
