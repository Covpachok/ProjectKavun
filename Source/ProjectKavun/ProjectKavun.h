// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EKavunAbilityID : uint8
{
	None UMETA(DisplayName = "None"),
	Confirm UMETA(DisplayName = "Confirm"),
	Cancel UMETA(DisplayName = "Cancel"),
	Shoot UMETA(DisplayName = "Shoot"),
	UseActiveItem UMETA(DisplayName = "Use Active Item"),
	UseTrinket UMETA(DisplayName = "Use Trinket"),
};
