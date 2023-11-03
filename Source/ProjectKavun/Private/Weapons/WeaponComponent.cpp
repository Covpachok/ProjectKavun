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
{
	PrimaryComponentTick.bCanEverTick = true;

	LastShotDelay            = 0.f;
	ProjectileVelocityFactor = 0.4f;
	KnockBack = 1;

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

void UWeaponComponent::Shoot(const FVector&                       Location, const FRotator& Rotation,
                             const UCharacterAttributesComponent* CharacterAttributes)
{
	const float ShotDelay = 1 / CharacterAttributes->GetAttribute(ECharacterAttributes_ProjectilesPerSecond);
	while ( LastShotDelay < ShotDelay )
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
	Projectile->SetRange(CharacterAttributes->GetAttribute(ECharacterAttributes_ProjectileRange));
	Projectile->SetDamage(CharacterAttributes->GetAttribute(ECharacterAttributes_Damage));
	Projectile->SetKnockBack(KnockBack);

	Projectile->Reload();
	Projectile->OnProjectileHit.BindUObject(this, &UWeaponComponent::OnProjectileHit);

	UProjectileMovementComponent* ProjectileMovement = Projectile->GetProjectileMovement();

	ProjectileMovement->Velocity = Projectile->GetActorForwardVector() *
	                               CharacterAttributes->GetAttribute(ECharacterAttributes_ProjectileSpeed) +
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
