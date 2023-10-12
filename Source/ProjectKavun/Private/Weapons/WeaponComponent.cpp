// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponComponent.h"

#include "ActorPoolComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectiles/ProjectileBase.h"

DEFINE_LOG_CATEGORY(WeaponComponentLog);

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	ShotDelay = 0.1f;
	LastShotDelay = 0.f;
	ProjectileRange = 1000.f;
	ProjectileSpeed = 1000.f;
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

void UWeaponComponent::Shoot()
{
	if ( LastShotDelay < ShotDelay )
	{
		return;
	}
	LastShotDelay = 0;

	if ( !IsValid(ProjectilePool) || !IsValid(OwnerCharacter) )
	{
		UE_LOG(WeaponComponentLog, Error, TEXT("ProjectilePool or OwnerCharacter is not valid"));
		return;
	}

	const FRotator Rotation = OwnerCharacter->GetActorRotation();
	const FVector  Location = OwnerCharacter->GetActorLocation();

	AActor* PoolActor;
	ProjectilePool->Pull(PoolActor);
	
	AProjectileBase* Projectile = Cast<AProjectileBase>(PoolActor);
	Projectile->SetActorLocationAndRotation(Location, Rotation);
	Projectile->SetRange(ProjectileRange);
	Projectile->Reload();
	
	UProjectileMovementComponent* ProjectileMovement = Projectile->GetProjectileMovement();
	ProjectileMovement->Velocity = Projectile->GetActorForwardVector() * ProjectileSpeed + (
		                               OwnerCharacter->GetMovementComponent()->Velocity * ProjectileVelocityFactor);
	ProjectileMovement->ProjectileGravityScale = 0;
}

void UWeaponComponent::ChangeProjectileClass(TSubclassOf<AProjectileBase> ProjectileClass)
{
	if(!IsValid(ProjectilePool))
	{
		return;
	}

	ProjectilePool->ChangeActorClass(ProjectileClass);
}

void UWeaponComponent::SetShotDelay(float NewDelay)
{
	ShotDelay = NewDelay;
}

