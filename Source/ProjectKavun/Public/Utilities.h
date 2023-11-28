#pragma once

#include "CoreMinimal.h"

UENUM()
enum EDirections : uint8
{
	EDirections_Up UMETA(DisplayName = "Up"),
	EDirections_Down UMETA(DisplayName = "Down"),
	EDirections_Left UMETA(DisplayName = "Left"),
	EDirections_Right UMETA(DisplayName = "Right"),
	EDirections_Count UMETA(Hidden)
};

const FIntPoint KIntPointUp    = {0, 1};
const FIntPoint KIntPointDown  = {0, -1};
const FIntPoint KIntPointLeft  = {-1, 0};
const FIntPoint KIntPointRight = {1, 0};

const FIntPoint KIntPointByDirection[EDirections_Count]
{
		KIntPointUp,
		KIntPointDown,
		KIntPointLeft,
		KIntPointRight
};

const TMap<FIntPoint, EDirections> KDirectionByPoint
{
		{KIntPointUp, EDirections_Up},
		{KIntPointDown, EDirections_Down},
		{KIntPointLeft, EDirections_Left},
		{KIntPointRight, EDirections_Right},
};
