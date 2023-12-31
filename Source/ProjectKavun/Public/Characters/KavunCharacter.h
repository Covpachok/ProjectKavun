// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "KavunCharacter.generated.h"

class UHealthComponent;
class AKavunCamera;
class UCharacterAttributesComponent;
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

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Kavun Character")
	void SetCameraActor(AKavunCamera* CameraActor) { CameraRef = CameraActor; }

	UFUNCTION(BlueprintCallable, Category = "Kavun Character")
	AKavunCamera* GetCameraActor() const { return CameraRef; }

	UFUNCTION(BlueprintCallable, Category = "Kavun Character|Components")
	UHealthComponent* GetHealthComponent() const { return HealthComponent; }

	UFUNCTION(BlueprintCallable, Category = "Kavun Character|Components")
	UWeaponComponent* GetWeaponComponent() const { return WeaponComponent; }
	

private:
	/*****************
	 * Input
	 *****************/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;


	/*****************
	 * Components
	 *****************/

	UPROPERTY(EditAnywhere, Category = "Components")
	USphereComponent* CameraCollider;

	UPROPERTY(EditAnywhere, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	UWeaponComponent* WeaponComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	UActorPoolComponent* ProjectilePool;

	UPROPERTY(VisibleInstanceOnly, Category = "Other")
	AKavunCamera* CameraRef;
};
