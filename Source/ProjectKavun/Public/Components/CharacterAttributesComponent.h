// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterAttributesComponent.generated.h"

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttributeValueChanged, const FKavunAttribute&, Attribute, float, OldValue, float, NewValue);


UENUM()
enum EKavunAttributesId : uint8
{
	EKavunAttributeId_MovementSpeed UMETA(DisplayName = "MovementSpeed"),
	EKavunAttributeId_Damage UMETA(DisplayName = "Damage"),
	EKavunAttributeId_DamageMultiplier UMETA(DisplayName = "DamageMultiplier"),
	EKavunAttributeId_ProjectilesPerSecond UMETA(DisplayName = "ProjectilesPerSecond"),
	EKavunAttributeId_ProjectilesPerSecondMultiplier UMETA(DisplayName = "ProjectilesPerSecondMultiplier"),
	EKavunAttributeId_ProjectileSpeed UMETA(DisplayName = "ProjectileSpeed"),
	EKavunAttributeId_ProjectileRange UMETA(DisplayName = "ProjectileRange"),
	EKavunAttributeId_Luck UMETA(DisplayName = "Luck"),
	EKavunAttributeId_Count UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FKavunAttribute
{
	GENERATED_BODY()

	// UPROPERTY(VisibleDefaultsOnly)
	// EKavunAttributesId Id;

	UPROPERTY(EditDefaultsOnly)
	// Min clamp value
	float Min = 0;

	UPROPERTY(EditDefaultsOnly)
	// Max clamp value
	float Max = 1;

	UPROPERTY(EditDefaultsOnly)
	// Base value, shouldn't change at runtime
	float Base = 1;

	UPROPERTY(VisibleAnywhere)
	// Current value calculated at runtime
	float Current = 0;

	UPROPERTY(VisibleAnywhere)
	// Difference between Base and Current
	float TotalChange = 0;

	UPROPERTY(VisibleAnywhere)
	// Last change to the attribute
	float LastChange = 0;
};

/*
 * Player/Enemy common attributes:
 * Health, Damage, MovementSpeed
 */

/*
 * Player specific attributes:
 * Size, Luck, Money
 */

/*
 * Projectile attributes:
 * Damage, Range, Speed, Size
 */

/*
 * Weapon attributes:
 * ShootingSpeed
 */

/**
 * Stores all player character stats
 */
UCLASS()
class PROJECTKAVUN_API UCharacterAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterAttributesComponent();

protected:
	virtual void BeginPlay() override;

public:
	void ChangeAttribute(FKavunAttribute &Attribute, float ChangeValue);

private:
	void DefaultRecalculateAttribute(FKavunAttribute &Attribute);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KavunCharacter|Attributes")
	FKavunAttribute Health;
};
