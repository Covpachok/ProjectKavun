// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/Projectile.h"

#include "Characters/KavunCharacterBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	// CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);

	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovementComponent->UpdatedComponent = CollisionComp;
	ProjectileMovementComponent->InitialSpeed = 3000.f;
	ProjectileMovementComponent->MaxSpeed = 3000.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;

	InitialLifeSpan = 0;

	PrevLocation      = FVector::ZeroVector;
	TravelledDistance = 0;

	KnockbackStrength = 1;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TravelledDistance += (GetActorLocation() - PrevLocation).Size();
	if ( TravelledDistance >= MaxRange )
	{
		SoftDestroy();
	}

	PrevLocation = GetActorLocation();
	LastVelocity = ProjectileMovementComponent->Velocity;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp,
                        AActor*              OtherActor,
                        UPrimitiveComponent* OtherComp,
                        FVector              NormalImpulse,
                        const FHitResult&    Hit)
{
	if ( !IsValid(OtherActor) )
	{
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("AProjectile::OnHit : Hit %s"), *OtherActor->GetName());
	OnProjectileHit.ExecuteIfBound(this, Hit.Location, OtherActor);

	AKavunCharacterBase* Character = Cast<AKavunCharacterBase>(OtherActor);
	if ( IsValid(Character) )
	{
		UE_LOG(LogTemp, Display, TEXT("AProjectile::OnHit : Hit Character %s"), *Character->GetName());
		HitCharacter(*Character);
	}

	SoftDestroy();
}

void AProjectile::HitCharacter(AKavunCharacterBase& Character)
{
	Character.TakeDamage(Damage, GetActorForwardVector(), KnockbackStrength);
}

void AProjectile::Reload()
{
	TravelledDistance = 0;
	PrevLocation      = GetActorLocation();
}

void AProjectile::Disable()
{
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);

	ProjectileMovementComponent->Deactivate();
	TravelledDistance = 0;
}

void AProjectile::Enable()
{
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
	SetActorHiddenInGame(false);

	ProjectileMovementComponent->Activate();
}

void AProjectile::OnPushed_Implementation()
{
	IPoolActor::OnPushed_Implementation();

	Disable();
}

void AProjectile::OnPulled_Implementation(UActorPoolComponent* ActorPool)
{
	IPoolActor::OnPulled_Implementation(ActorPool);

	ActorPoolRef = ActorPool;

	Enable();
}

void AProjectile::SoftDestroy()
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
