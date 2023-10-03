// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "KavunCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class AProjectileBase;

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

public:
	void Move(const FInputActionValue &Value);
	void Shoot(const FInputActionValue &Value);
	
private:
	void SpawnProjectile();

private:

	/* Input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	
	/* Projectile */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<AProjectileBase> ProjectileClass;

	UPROPERTY(EditAnywhere, Category=Projectile)
	float ShootingSpeed;
	
	UPROPERTY(EditAnywhere, Category=Projectile)
	float ProjectileVelocityFactor;
	
	UPROPERTY(EditAnywhere, Category=Projectile)
	float ProjectileRangeFactor;

	UPROPERTY(EditAnywhere, Category=Projectile)
	float ProjectileAngle;
	
	UPROPERTY(EditAnywhere, Category=Projectile)
	float ProjectilesAmount;
	
	int ProjectileDeltaAngle;

	float LastTimeShoot;
};
