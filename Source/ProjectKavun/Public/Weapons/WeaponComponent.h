// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

class UCharacterAttributesComponent;
class AKavunCharacter;
class UActorPoolComponent;
class AProjectile;

UDELEGATE(BlueprintCallable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnProjectileHitObjectSignature, UWeaponComponent *, Emitter, AProjectile *, Projectile, AActor *, HitActor, const FVector &, Location);

DECLARE_LOG_CATEGORY_EXTERN(WeaponComponentLog, Log, All);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTKAVUN_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnProjectileHitObjectSignature OnProjectileHitObject;

public:
	UWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float                        DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Shoot(const FVector &Location, const FRotator &Rotation, const UCharacterAttributesComponent *CharacterAttributes);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void ChangeProjectileClass(TSubclassOf<AProjectile> ProjectileClass);

	virtual void OnProjectileHit(AProjectile *Projectile, const FVector &Location, AActor *OtherActor);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AProjectile *PullProjectile();

protected:
	UPROPERTY(EditAnywhere, Category = "Projectile")
	float ProjectileVelocityFactor;

	float LastShotDelay;

	UPROPERTY()
	UActorPoolComponent* ProjectilePool;

	UPROPERTY()
	ACharacter* OwnerCharacter;
};
