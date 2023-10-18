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

	FChangeMovementSpeedCallbackSignature                  ChangeMovementSpeedCallbacks;
	FChangeDamageCallbackSignature                         ChangeDamageCallbacks;
	FChangeDamageMultiplierCallbackSignature               ChangeDamageMultiplierCallbacks;
	FChangeProjectilesPerSecondCallbackSignature           ChangeProjectilesPerSecondCallbacks;
	FChangeProjectilesPerSecondMultiplierCallbackSignature ChangeProjectilesPerSecondMultiplierCallbacks;
	FChangeProjectileRangeCallbackSignature                ChangeProjectileRangeCallbacks;
	FChangeProjectileSpeedCallbackSignature                ChangeProjectileSpeedCallbacks;
	// FChangeLuckCallbackSignature                           ChangeLuckCallbacks;

public:
	UCharacterStatsComponent();

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	void RecalculateAll();

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	FString ToString() const;

protected:
	virtual void BeginPlay() override;

	/** --- MOVEMENT SECTION --- */
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

	/** --- DAMAGE SECTION --- */
public:
	UFUNCTION(BlueprintGetter)
	float GetDamage() const { return Damage; }

	UFUNCTION(BlueprintGetter)
	float GetDamageMultiplier() const { return DamageMultiplier; }

	UFUNCTION(BlueprintGetter)
	float GetDamageLastChange() const { return DamageLastChange; }

	void RecalculateDamage();

private:
	UPROPERTY(EditAnywhere, Category = "Character Stats|Damage", meta = (ClampMin=0.1, UIMin=0.1))
	float DamageBase;

	UPROPERTY(EditAnywhere, Category = "Character Stats|Damage", meta = (ClampMin=0.1, UIMin=0.1))
	float DamageMultiplierBase;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter="GetDamage", Category = "Character Stats|Damage")
	float Damage;

	UPROPERTY(VisibleInstanceOnly, Category = "Character Stats|Damage")
	float DamagePlain;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter="GetDamageMultiplier", Category = "Character Stats|Damage")
	float DamageMultiplier;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter="GetDamageLastChange", Category = "Character Stats|Damage")
	float DamageLastChange;

	/** --- PPS SECTION --- */
public:
	UFUNCTION(BlueprintGetter)
	float GetProjectilesPerSecond() const { return ProjectilesPerSecond; }

	UFUNCTION(BlueprintGetter)
	float GetProjectilesDelay() const { return ProjectilesDelay; }

	UFUNCTION(BlueprintGetter)
	float GetProjectilesPerSecondMultiplier() const { return ProjectilesPerSecondMultiplier; }

	UFUNCTION(BlueprintGetter)
	float GetProjectilesPerSecondLastChange() const { return ProjectilesPerSecondLastChange; }

	void RecalculateProjectilesPerSecond();

private:
	UPROPERTY(EditAnywhere, Category = "Character Stats|ProjectilesPerSecond", meta = (ClampMin=0.1, UIMin=0.1))
	float ProjectilesPerSecondBase;

	UPROPERTY(EditAnywhere, Category = "Character Stats|ProjectilesPerSecond", meta = (ClampMin=0.1, UIMin=0.1))
	float ProjectilesPerSecondMultiplierBase;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter="GetProjectilesDelay",
		Category = "Character Stats|ProjectilesPerSecond")
	float ProjectilesDelay;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter="GetProjectilesPerSecond",
		Category = "Character Stats|ProjectilesPerSecond")
	float ProjectilesPerSecond;

	UPROPERTY(VisibleInstanceOnly, Category = "Character Stats|ProjectilesPerSecond")
	float ProjectilesPerSecondPlain;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter="GetProjectilesPerSecondMultiplier",
		Category = "Character Stats|ProjectilesPerSecond")
	float ProjectilesPerSecondMultiplier;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter="GetProjectilesPerSecondLastChange",
		Category = "Character Stats|ProjectilesPerSecond")
	float ProjectilesPerSecondLastChange;

	/** --- PROJECTILE RANGE SECTION --- */
public:
	UFUNCTION(BlueprintGetter)
	float GetProjectileRange() const { return ProjectileRange; }

	UFUNCTION(BlueprintGetter)
	float GetProjectileRangeLastChange() const { return ProjectileRangeLastChange; }

	void RecalculateProjectileRange();

private:
	UPROPERTY(EditAnywhere, Category = "Character Stats", meta = (ClampMin=0.1, UIMin=0.1))
	float ProjectileRangeBase;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter="GetProjectileRange", Category = "Character Stats")
	float ProjectileRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter="GetProjectileRangeLastChange", Category = "Character Stats")
	float ProjectileRangeLastChange;

	/** --- PROJECTILE SPEED SECTION --- */
public:
	UFUNCTION(BlueprintGetter)
	float GetProjectileSpeed() const { return ProjectileSpeed; }

	UFUNCTION(BlueprintGetter)
	float GetProjectileSpeedLastChange() const { return ProjectileSpeedLastChange; }

	void RecalculateProjectileSpeed();

private:
	UPROPERTY(EditAnywhere, Category = "Character Stats", meta = (ClampMin=0.1, UIMin=0.1))
	float ProjectileSpeedBase;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter="GetProjectileSpeed", Category = "Character Stats")
	float ProjectileSpeed;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter="GetProjectileSpeedLastChange", Category = "Character Stats")
	float ProjectileSpeedLastChange;
};
