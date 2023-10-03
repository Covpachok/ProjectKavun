// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/ProjectileBase.h"

#include "Components/SphereComponent.h"
#include "Enemies/Enemy.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectileBase::AProjectileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	// Set up a notification for when this component hits something blocking
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 5.0f;
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp,
                            AActor*              OtherActor,
                            UPrimitiveComponent* OtherComp,
                            FVector              NormalImpulse,
                            const FHitResult&    Hit)
{
	// AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	// if ( Enemy != nullptr )
	// {
		// Enemy->TakeHit();
	// }

	Destroy();
}
