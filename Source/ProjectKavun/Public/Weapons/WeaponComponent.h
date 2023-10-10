// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

class AKavunCharacter;
class UActorPoolComponent;
class AProjectileBase;

DECLARE_LOG_CATEGORY_EXTERN(WeaponComponentLog, Log, All);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTKAVUN_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float                        DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual void Shoot();

	UFUNCTION(BlueprintCallable)
	virtual void ChangeProjectileClass(TSubclassOf<AProjectileBase> ProjectileClass);

	UFUNCTION(BlueprintCallable)
	virtual void SetShotDelay(float NewDelay);

protected:
	UPROPERTY(EditAnywhere, Category= "Projectile")
	float ShotDelay;

	UPROPERTY(EditAnywhere, Category= "Projectile")
	float ProjectileRange;
	
	UPROPERTY(EditAnywhere, Category= "Projectile")
	float ProjectileVelocityFactor;

	float LastShotDelay;

	UPROPERTY()
	UActorPoolComponent* ProjectilePool;

	UPROPERTY()
	ACharacter* OwnerCharacter;
};
