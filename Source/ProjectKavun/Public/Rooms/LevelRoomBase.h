// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelRoomBase.generated.h"

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
	FIntVector2 OccupiedTilesLocations[4];

	bool bLeftRightAccessible = true;
	bool bUpDownAccessible    = true;
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

	void ConstructRoom();

private:
	/* Meshes */
	UPROPERTY(EditAnywhere, Category = "Meshes")
	UStaticMeshComponent* VerticalWallMesh;

	UPROPERTY(EditAnywhere, Category = "Meshes")
	UStaticMeshComponent* VerticalWallDoorwayMesh;

	UPROPERTY(EditAnywhere, Category = "Meshes")
	UStaticMeshComponent* HorizontalWallMesh;

	UPROPERTY(EditAnywhere, Category = "Meshes")
	UStaticMeshComponent* HorizontalWallDoorwayMesh;

	UPROPERTY(EditAnywhere, Category = "Meshes")
	UStaticMeshComponent* DefaultDoorMesh;

	UPROPERTY(EditAnywhere, Category = "Meshes")
	UStaticMeshComponent* FloorMesh;


	/* Properties */

	UPROPERTY(VisibleAnywhere, Category = "Room details")
	ERoomShape RoomShape;

	UPROPERTY(VisibleAnywhere, Category = "Room details")
	ERoomType RoomType;
};
