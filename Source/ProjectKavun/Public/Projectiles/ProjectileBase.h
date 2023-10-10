// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorPoolComponent.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class PROJECTKAVUN_API AProjectileBase : public AActor, public IPoolActor
{
	GENERATED_BODY()

public:
	AProjectileBase();

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

	virtual void OnPushed_Implementation() override;
	virtual void OnPulled_Implementation(UActorPoolComponent* ActorPool) override;

protected:
	virtual void SoftDestroy();

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float MaxRange;

	UPROPERTY()
	UActorPoolComponent *ActorPoolRef;
	
	float TravelledDistance;
	FVector PrevLocation;
};
