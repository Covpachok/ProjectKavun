// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/SphereComponent.h"
#include "KavunCharacterBase.h"
#include "PlayerCharacter.generated.h"

class UHealthComponent;
class AKavunCamera;
class UCharacterAttributesComponent;
class UWeaponComponent;
class UActorPoolComponent;
class UInputMappingContext;
class UInputAction;
class AProjectile;

UCLASS()
class PROJECTKAVUN_API APlayerCharacter : public AKavunCharacterBase
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;


	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	                   UPrimitiveComponent* OtherComp, FVector    NormalImpulse, const FHitResult& Hit) override;

	void Move(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);

	virtual void TakeDamage(float Damage, const FVector& Impact, float KnockbackStrength) override;


	/*****************
	 * Components
	 *****************/

	UFUNCTION(BlueprintCallable, Category = "Player|Camera")
	void SetCameraActor(AKavunCamera* CameraActor) { CameraRef = CameraActor; }

	UFUNCTION(BlueprintCallable, Category = "Player|Camera")
	AKavunCamera* GetCameraActor() const { return CameraRef; }

	UFUNCTION(BlueprintCallable, Category = "Player|Components")
	UWeaponComponent* GetWeaponComponent() const { return WeaponComponent; }

	/*****************
	 * Pickups
	 *****************/

	UFUNCTION(BlueprintCallable, Category = "Player|Pickups")
	void AddMoney(int Number);

	UFUNCTION(BlueprintCallable, Category = "Player|Pickups")
	void AddKeys(int Number);

	UFUNCTION(BlueprintCallable, Category = "Player|Pickups")
	void SubtractMoney(int Number) { AddMoney(-Number); }

	UFUNCTION(BlueprintCallable, Category = "Player|Pickups")
	void SubtractKeys(int Number) { AddKeys(-Number); }

	UFUNCTION(BlueprintGetter, Category = "Player|Pickups")
	int GetMoney() const { return CurrentMoney; }

	UFUNCTION(BlueprintGetter, Category = "Player|Pickups")
	int GetKeys() const { return CurrentKeys; }

private:
	void InitAbilityActorInfo();

	/** This functions exists because when PlayerStart spawns player,
	 * it doesnt generate overlap events with collisions */
	void DetectorOverlap();

private:
	/*****************
	 * Input
	 *****************/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;


	/*****************
	 * Components
	 *****************/

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<USphereComponent> CameraCollider;


	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UWeaponComponent> WeaponComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UActorPoolComponent> ProjectilePool;

	/*****************
	 * Camera
	 *****************/

	UPROPERTY(EditAnywhere, Category = "Other")
	TSubclassOf<AKavunCamera> CameraClass;

	TObjectPtr<AKavunCamera> CameraRef;

	/*****************
	 * Pickup Counters
	 *****************/

	UPROPERTY(EditAnywhere, BlueprintGetter = "GetMoney", Category = "Pickups", meta = (UIMin = 0, ClampMin = 0))
	int CurrentMoney;

	UPROPERTY(EditAnywhere, BlueprintGetter = "GetKeys", Category = "Pickups", meta = (UIMin = 0, ClampMin = 0))
	int CurrentKeys;
};
