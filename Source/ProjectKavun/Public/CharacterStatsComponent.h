// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStatsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterStatsChangedCallbackSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeMovementSpeedCallbackSignature, float &, MovementSpeed);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeDamageCallbackSignature, float &, Damage);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeDamageMultiplierCallbackSignature, float &, DamageMultiplier);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeProjectilesPerSecondCallbackSignature, float &, ProjectilesPerSecond)
;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeProjectilesPerSecondMultiplierCallbackSignature, float &,
                                            ProjectilesPerSecondMultiplier);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeProjectileRangeCallbackSignature, float &, ProjectileRange);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeProjectileSpeedCallbackSignature, float &, ProjectileSpeed);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeLuckCallbackSignature, float &, Luck);

/**
 * 
 */
UCLASS()
class PROJECTKAVUN_API UCharacterStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnCharacterStatsChangedCallbackSignature OnCharacterStatsChanged;

	FChangeMovementSpeedCallbackSignature    ChangeMovementSpeedCallbacks;
	FChangeDamageCallbackSignature           ChangeDamageCallbacks;
	FChangeDamageMultiplierCallbackSignature ChangeDamageMultiplierCallbacks;
	// FChangeProjectilesPerSecondCallbackSignature           ChangeProjectilesPerSecondCallbacks;
	// FChangeProjectilesPerSecondMultiplierCallbackSignature ChangeProjectilesPerSecondMultiplierCallbacks;
	// FChangeProjectileRangeCallbackSignature                ChangeProjectileRangeCallbacks;
	// FChangeProjectileSpeedCallbackSignature                ChangeProjectileSpeedCallbacks;
	// FChangeLuckCallbackSignature                           ChangeLuckCallbacks;

public:
	UCharacterStatsComponent();

	void RecalculateAll()
	{
		RecalculateMovementSpeed();
		RecalculateDamage();
	}

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintGetter)
	float GetMovementSpeed() const { return MovementSpeed; }

	UFUNCTION(BlueprintGetter)
	float GetMovementSpeedLastChange() const { return MovementSpeedLastChange; }

	void RecalculateMovementSpeed()
	{
		const float OldMovementSpeed = MovementSpeed;
		MovementSpeed                = MovementSpeedBase;

		ChangeMovementSpeedCallbacks.Broadcast(MovementSpeed);

		MovementSpeedLastChange = MovementSpeed - OldMovementSpeed;

		OnCharacterStatsChanged.Broadcast();
	}

private:
	UPROPERTY(EditAnywhere, Category = "Character Stats", meta = (ClampMin=0.1, ClampMax=2, UIMin=0.1, UIMax=2))
	float MovementSpeedBase;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter="GetMovementSpeed", Category = "Character Stats")
	float MovementSpeed;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter="GetMovementSpeedLastChange", Category = "Character Stats")
	float MovementSpeedLastChange;

public:
	UFUNCTION(BlueprintGetter)
	float GetDamage() const { return Damage; }

	UFUNCTION(BlueprintGetter)
	float GetDamageMultiplier() const { return DamageMultiplier; }

	UFUNCTION(BlueprintGetter)
	float GetDamageLastChange() const { return DamageLastChange; }

	void RecalculateDamage()
	{
		const float OldDamage = Damage;
		DamagePlain           = DamageBase;
		DamageMultiplier      = DamageMultiplierBase;

		ChangeDamageCallbacks.Broadcast(DamagePlain);
		ChangeDamageMultiplierCallbacks.Broadcast(DamageMultiplier);

		Damage           = DamagePlain * DamageMultiplier;
		DamageLastChange = Damage - OldDamage;

		OnCharacterStatsChanged.Broadcast();
	}

private:
	UPROPERTY(EditAnywhere, Category = "Character Stats|Damage", meta = (ClampMin=0.1, UIMin=0.1))
	float DamageBase;

	UPROPERTY(EditAnywhere, Category = "Character Stats|Damage", meta = (ClampMin=0.1, UIMin=0.1))
	float DamageMultiplierBase;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter="GetDamage", Category = "Character Stats|Damage")
	float Damage;

	UPROPERTY(VisibleInstanceOnly, Category = "Character Stats|Damage", meta = (ClampMin=0.1, UIMin=0.1))
	float DamagePlain;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter="GetDamageMultiplier", Category = "Character Stats|Damage")
	float DamageMultiplier;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter="GetDamageLastChange", Category = "Character Stats|Damage")
	float DamageLastChange;
};
