// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatsComponent.h"

UCharacterStatsComponent::UCharacterStatsComponent()
{
	MovementSpeedBase       = 1.f;
	
	MovementSpeed           = 0.f;
	MovementSpeedLastChange = 0.f;

	DamageMultiplierBase = 1.f;
	DamageBase           = 3.f;
	
	Damage           = 0.f;
	DamagePlain      = 0.f;
	DamageMultiplier = 0.f;
}

void UCharacterStatsComponent::BeginPlay()
{
	Super::BeginPlay();
}
