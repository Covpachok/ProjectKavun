// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxHealth                 = 10;
	CurrentHealth             = 0;
	InvincibilityTimeAfterHit = 0.32f;

	HitTimer = 0;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

void UHealthComponent::TickComponent(float                        DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HitTimer += DeltaTime;
}

void UHealthComponent::TakeDamage(float Amount)
{
	if ( HitTimer < InvincibilityTimeAfterHit )
	{
		return;
	}

	HitTimer = 0;

	float OldHealth = CurrentHealth;
	CurrentHealth -= Amount;
	if ( CurrentHealth <= 0 )
	{
		CurrentHealth = 0;
	}

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, CurrentHealth - OldHealth, 0);
}

void UHealthComponent::Heal(float Amount)
{
	float OldHealth = CurrentHealth;
	CurrentHealth += Amount;
	if ( CurrentHealth >= MaxHealth )
	{
		CurrentHealth = MaxHealth;
	}

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, CurrentHealth - OldHealth, 0);
}

void UHealthComponent::SetMaxHealth(float NewValue)
{
	float OldMaxHealth = MaxHealth;
	MaxHealth          = NewValue;

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, 0, MaxHealth - OldMaxHealth);
}

void UHealthComponent::SetMaxHealthAndHeal(float NewValue)
{
	float OldHealth    = CurrentHealth;
	float OldMaxHealth = MaxHealth;
	MaxHealth          = NewValue;
	CurrentHealth      = MaxHealth;

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, CurrentHealth - OldHealth, MaxHealth - OldMaxHealth);
}

void UHealthComponent::IncreaseMaxHealth(float Value)
{
	float OldMaxHealth = MaxHealth;
	MaxHealth += Value;

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, 0, MaxHealth - OldMaxHealth);
}
