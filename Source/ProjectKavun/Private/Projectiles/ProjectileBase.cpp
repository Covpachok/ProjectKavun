// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/ProjectileBase.h"

#include "Components/SphereComponent.h"
#include "Enemies/Enemy.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Iris/Core/BitTwiddling.h"

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
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovementComponent->UpdatedComponent = CollisionComp;
	ProjectileMovementComponent->InitialSpeed = 3000.f;
	ProjectileMovementComponent->MaxSpeed = 3000.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 0;

	PrevLocation      = FVector::ZeroVector;
	TravelledDistance = 0;
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TravelledDistance += (GetActorLocation() - PrevLocation).Size();
	if ( TravelledDistance >= MaxRange )
	{
		SoftDestroy();
	}

	PrevLocation = GetActorLocation();
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp,
                            AActor*              OtherActor,
                            UPrimitiveComponent* OtherComp,
                            FVector              NormalImpulse,
                            const FHitResult&    Hit)
{
	SoftDestroy();
}

void AProjectileBase::SetRange(float NewRange)
{
	MaxRange = NewRange;
}

void AProjectileBase::Reload()
{
	TravelledDistance = 0;
	PrevLocation      = GetActorLocation();
}

void AProjectileBase::Disable()
{
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);

	ProjectileMovementComponent->Deactivate();
	TravelledDistance = 0;
}

void AProjectileBase::Enable()
{
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
	SetActorHiddenInGame(false);

	ProjectileMovementComponent->Activate();
}

void AProjectileBase::OnPushed_Implementation()
{
	IPoolActor::OnPushed_Implementation();

	Disable();
}

void AProjectileBase::OnPulled_Implementation(UActorPoolComponent* ActorPool)
{
	IPoolActor::OnPulled_Implementation(ActorPool);

	ActorPoolRef = ActorPool;

	Enable();
}

void AProjectileBase::SoftDestroy()
{
	if ( IsValid(ActorPoolRef) )
	{
		if ( ActorPoolRef->Push(this) == false )
		{
			Destroy();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ProjectileBase: Projectile pool not found."));
		Destroy();
	}
}
