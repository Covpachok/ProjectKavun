// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxHealth = 10;
	CurrentHealth = 0;
	InvincibilityTimeAfterHit = 0.32f;
	
	HitTimer = 0;
}


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HitTimer += DeltaTime;
}

void UHealthComponent::TakeDamage(float Amount)
{
	if(HitTimer < InvincibilityTimeAfterHit)
	{
		return;
	}

	HitTimer = 0;
	
	CurrentHealth -= Amount;
	if(CurrentHealth <= 0)
	{
		CurrentHealth = 0;
	}

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UHealthComponent::Heal(float Amount)
{
	CurrentHealth += Amount;
	if(CurrentHealth >= MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
	
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UHealthComponent::SetMaxHealth(float NewValue)
{
	MaxHealth = NewValue;

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UHealthComponent::SetMaxHealthAndHeal(float NewValue)
{
	MaxHealth = NewValue;
	CurrentHealth = MaxHealth;
	
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UHealthComponent::IncreaseMaxHealth(float Value)
{
	MaxHealth += Value;
	
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

