// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CharacterStatsComponent.h"

UCharacterStatsComponent::UCharacterStatsComponent()
	: MovementSpeedBase(1),
	  MovementSpeed(0),
	  MovementSpeedLastChange(0),
	  DamageBase(3),
	  DamageMultiplierBase(0),
	  Damage(0),
	  DamagePlain(0),
	  DamageMultiplier(1),
	  DamageLastChange(0),
	  ProjectilesPerSecondBase(1),
	  ProjectilesPerSecondMultiplierBase(1),
	  ProjectilesDelay(0),
	  ProjectilesPerSecond(0),
	  ProjectilesPerSecondPlain(0),
	  ProjectilesPerSecondMultiplier(0),
	  ProjectilesPerSecondLastChange(0),
	  ProjectileRangeBase(1000),
	  ProjectileRange(0),
	  ProjectileRangeLastChange(0),
	  ProjectileSpeedBase(1000),
	  ProjectileSpeed(0),
	  ProjectileSpeedLastChange(0)
{
}

void UCharacterStatsComponent::RecalculateAll()
{
	RecalculateMovementSpeed();
	RecalculateDamage();
	RecalculateProjectilesPerSecond();
	RecalculateProjectileSpeed();
	RecalculateProjectileRange();
}

FString UCharacterStatsComponent::ToString() const
{
	FString Result;
	Result += FString::Printf(TEXT("MSP: %02.2f\n"), MovementSpeed);
	Result += FString::Printf(TEXT("DMG: %02.2f (%01.2f%%)\n"), Damage, DamageMultiplier);
	Result += FString::Printf(TEXT("PPS: %02.2f (%01.2f%%)\n"), ProjectilesPerSecond, ProjectilesPerSecondMultiplier);
	Result += FString::Printf(TEXT("PRG: %.0f\n"), ProjectileRange);
	Result += FString::Printf(TEXT("PSP: %.0f\n"), ProjectileSpeed);
	return Result;
}

void UCharacterStatsComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCharacterStatsComponent::RecalculateDamage()
{
	const float OldDamage = Damage;
	DamagePlain           = DamageBase;
	DamageMultiplier      = DamageMultiplierBase;

	ChangeDamageCallbacks.Broadcast(DamagePlain);
	ChangeDamageMultiplierCallbacks.Broadcast(DamageMultiplier);

	Damage           = DamagePlain * DamageMultiplier;
	DamageLastChange = Damage - OldDamage;

	OnCharacterStatsChanged.Broadcast();
}

void UCharacterStatsComponent::RecalculateProjectilesPerSecond()
{
	const float OldProjectilesPerSecond = ProjectilesPerSecond;
	ProjectilesPerSecondPlain           = ProjectilesPerSecondBase;
	ProjectilesPerSecondMultiplier      = ProjectilesPerSecondMultiplierBase;

	ChangeProjectilesPerSecondCallbacks.Broadcast(ProjectilesPerSecondPlain);
	ChangeProjectilesPerSecondMultiplierCallbacks.Broadcast(ProjectilesPerSecondMultiplier);

	ProjectilesPerSecond           = ProjectilesPerSecondPlain * ProjectilesPerSecondMultiplier;
	ProjectilesDelay               = 1 / ProjectilesPerSecond;
	ProjectilesPerSecondLastChange = ProjectilesPerSecond - OldProjectilesPerSecond;

	OnCharacterStatsChanged.Broadcast();
}

void UCharacterStatsComponent::RecalculateProjectileRange()
{
	const float OldProjectileRange = ProjectileRange;
	ProjectileRange                = ProjectileRangeBase;

	ChangeProjectileRangeCallbacks.Broadcast(ProjectileRange);

	ProjectileRangeLastChange = ProjectileRange - OldProjectileRange;

	OnCharacterStatsChanged.Broadcast();
}

void UCharacterStatsComponent::RecalculateProjectileSpeed()
{
	const float OldProjectileSpeed = ProjectileSpeed;
	ProjectileSpeed                = ProjectileSpeedBase;

	ChangeProjectileSpeedCallbacks.Broadcast(ProjectileSpeed);

	ProjectileSpeedLastChange = ProjectileSpeed - OldProjectileSpeed;

	OnCharacterStatsChanged.Broadcast();
}
