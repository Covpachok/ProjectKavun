// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "KavunCharacter.generated.h"

class UCharacterStatsComponent;
class UWeaponComponent;
class UActorPoolComponent;
class UInputMappingContext;
class UInputAction;
class AProjectile;

UCLASS()
class PROJECTKAVUN_API AKavunCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AKavunCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);

private:
	void SpawnProjectile();

	UFUNCTION()
	void OnStatsChanged();

	UFUNCTION()
	void DamageChangeCallback1(float& Damage) { Damage += 6; }

	UFUNCTION()
	void DamageChangeCallback2(float& Damage) { Damage += 3; }

	UFUNCTION()
	void DamageMultiplierChangeCallback1(float& DamageMultiplier) { DamageMultiplier += 0.25f; }

	UFUNCTION()
	void DamageMultiplierChangeCallback2(float& DamageMultiplier) { DamageMultiplier += 0.75f; }

private:
	/* Input */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/**
	 * STATS
	 */

	UPROPERTY(EditAnywhere)
	UCharacterStatsComponent* CharacterStats;

	UPROPERTY(EditAnywhere)
	UWeaponComponent* WeaponComponent;

	UPROPERTY(EditAnywhere)
	UActorPoolComponent* ProjectilePool;

	/* Projectile */

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float ShootingSpeed;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float ProjectileVelocityFactor;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float ProjectileRangeFactor;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float ProjectileAngle;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float ProjectilesAmount;

	int ProjectileDeltaAngle;

	UPROPERTY(VisibleInstanceOnly, Category = "Projectile")
	int ProjectilesSpawned;

	float LastTimeShoot;
};
