// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorPoolComponent.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class AProjectile;

DECLARE_DELEGATE_ThreeParams(FOnProjectileHitSignature, AProjectile *, const FVector &, AActor *)

UCLASS()
class PROJECTKAVUN_API AProjectile : public AActor, public IPoolActor
{
	GENERATED_BODY()

public:
	FOnProjectileHitSignature OnProjectileHit;

public:
	AProjectile();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp,
	           AActor*              OtherActor,
	           UPrimitiveComponent* OtherComp,
	           FVector              NormalImpulse,
	           const FHitResult&    Hit);

	void SetRange(float NewRange);

	USphereComponent*             GetCollisionComp() const { return CollisionComp; }
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovementComponent; }

	virtual void Reload();

	virtual void Disable();
	virtual void Enable();

	// ProjectileHit(AProjectile *Projectile, const FVector &HitLocation, AActor *OtherActor)

	virtual void OnPushed_Implementation() override;
	virtual void OnPulled_Implementation(UActorPoolComponent* ActorPool) override;

protected:
	/**
	 * Should be called instead of Destroy().
	 * Returns projectile to the ActorPool if it exists, destroys actor otherwise.
	 */
	void SoftDestroy();

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float MaxRange;

	UPROPERTY()
	UActorPoolComponent* ActorPoolRef;

	float   TravelledDistance;
	FVector PrevLocation;
};
