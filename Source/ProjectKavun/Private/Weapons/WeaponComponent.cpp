// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponComponent.h"

#include "Components/ActorPoolComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectiles/Projectile.h"
#include "Components/CharacterAttributesComponent.h"

DEFINE_LOG_CATEGORY(WeaponComponentLog);

UWeaponComponent::UWeaponComponent()
	: ProjectileDamage(3),
	  ProjectileShotDelay(0.5f),
	  ProjectileRange(3000),
	  ProjectileSpeed(850),
	  ProjectileSize(1),
	  ProjectileVelocityFactor(0.25f),
	  ProjectileKnockBack(2),
	  LastShotDelay(0),
	  ProjectilePool(nullptr),
	  OwnerCharacter(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if ( !IsValid(OwnerCharacter) )
	{
		UE_LOG(WeaponComponentLog, Error, TEXT("Owner is not a Character"));
		return;
	}

	ProjectilePool = OwnerCharacter->GetComponentByClass<UActorPoolComponent>();
	if ( !IsValid(ProjectilePool) )
	{
		UE_LOG(WeaponComponentLog, Error, TEXT("ActorPoolComponent not found on the owner"));
	}
}


void UWeaponComponent::TickComponent(float                        DeltaTime,
                                     ELevelTick                   TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	LastShotDelay += DeltaTime;
}

void UWeaponComponent::Shoot(const FVector& Location, const FRotator& Rotation)
{
	while ( LastShotDelay < ProjectileShotDelay )
	{
		return;
	}
	LastShotDelay = 0;

	if ( !IsValid(ProjectilePool) || !IsValid(OwnerCharacter) )
	{
		UE_LOG(WeaponComponentLog, Error,
		       TEXT("UWeaponComponent::Shoot : ProjectilePool or OwnerCharacter is not valid."));
		return;
	}

	AProjectile* Projectile = PullProjectile();
	if ( !IsValid(Projectile) )
	{
		UE_LOG(WeaponComponentLog, Error, TEXT("UWeaponComponent::Shoot : Pulled projectile is invalid."))
		return;
	}

	Projectile->SetActorLocationAndRotation(Location, Rotation);
	Projectile->SetRange(ProjectileRange);
	Projectile->SetDamage(ProjectileDamage);
	Projectile->SetKnockBack(ProjectileKnockBack);
	Projectile->SetActorScale3D(FVector::OneVector * ProjectileSize);

	Projectile->Reload();
	Projectile->OnProjectileHit.BindUObject(this, &UWeaponComponent::OnProjectileHit);

	UProjectileMovementComponent* ProjectileMovement = Projectile->GetProjectileMovement();

	ProjectileMovement->Velocity = Projectile->GetActorForwardVector() * ProjectileSpeed +
	                               (OwnerCharacter->GetMovementComponent()->Velocity * ProjectileVelocityFactor);

	ProjectileMovement->ProjectileGravityScale = 0;
}

void UWeaponComponent::ChangeProjectileClass(TSubclassOf<AProjectile> ProjectileClass)
{
	if ( !IsValid(ProjectilePool) )
	{
		UE_LOG(WeaponComponentLog, Warning, TEXT("UWeaponComponent::ChangeProjectileClass : Class is invalid."))
		return;
	}

	ProjectilePool->ChangeActorClass(ProjectileClass);
}

void UWeaponComponent::OnProjectileHit(AProjectile* Projectile, const FVector& Location, AActor* OtherActor)
{
	UE_LOG(WeaponComponentLog, Display, TEXT("Projectile hit %s at %s"), *OtherActor->GetName(), *Location.ToString());
	OnProjectileHitObject.Broadcast(this, Projectile, OtherActor, Location);
}

AProjectile* UWeaponComponent::PullProjectile()
{
	AActor* PoolActor;
	ProjectilePool->Pull(PoolActor);
	AProjectile* Projectile = Cast<AProjectile>(PoolActor);
	return Projectile;
}
