// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponComponent.h"

#include "Components/ActorPoolComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectiles/Projectile.h"
#include "Components/CharacterStatsComponent.h"

DEFINE_LOG_CATEGORY(WeaponComponentLog);

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	ShotDelay                = 0.1f;
	LastShotDelay            = 0.f;
	ProjectileRange          = 1000.f;
	ProjectileSpeed          = 1000.f;
	ProjectileVelocityFactor = 0.4f;

	ProjectilePool = nullptr;
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

void UWeaponComponent::Shoot(const FVector& Location, const FRotator& Rotation, const UCharacterStatsComponent* Stats)
{
	if ( LastShotDelay < Stats->GetProjectilesDelay() )
	{
		UE_LOG(WeaponComponentLog, Display, TEXT("UWeaponComponent::Shoot :  %02.2f/%02.2f"), LastShotDelay, Stats->GetProjectilesDelay())
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
	Projectile->SetRange(Stats->GetProjectileRange());
	Projectile->SetDamage(Stats->GetDamage());

	Projectile->Reload();
	Projectile->OnProjectileHit.BindUObject(this, &UWeaponComponent::OnProjectileHit);

	UProjectileMovementComponent* ProjectileMovement = Projectile->GetProjectileMovement();

	ProjectileMovement->Velocity = Projectile->GetActorForwardVector() * Stats->GetProjectileSpeed() +
	                               OwnerCharacter->GetMovementComponent()->Velocity * ProjectileVelocityFactor;

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

void UWeaponComponent::SetShotDelay(float NewDelay)
{
	ShotDelay = NewDelay;
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
