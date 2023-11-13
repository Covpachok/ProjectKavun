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
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;


	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	                   UPrimitiveComponent* OtherComp, FVector    NormalImpulse, const FHitResult& Hit) override;

	void Move(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Kavun Character")
	void SetCameraActor(AKavunCamera* CameraActor) { CameraRef = CameraActor; }

	UFUNCTION(BlueprintCallable, Category = "Kavun Character")
	AKavunCamera* GetCameraActor() const { return CameraRef; }

	UFUNCTION(BlueprintCallable, Category = "Kavun Character|Components")
	UWeaponComponent* GetWeaponComponent() const { return WeaponComponent; }

	virtual void TakeDamage(float Damage, const FVector& Impact, float KnockbackStrength) override;

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

	UPROPERTY(VisibleInstanceOnly, Category = "Other")
	TObjectPtr<AKavunCamera> CameraRef;
};
