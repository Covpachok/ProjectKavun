﻿// Copyright Covpachok


#include "AbilitySystem/KavunAbilitySystemComponent.h"


UKavunAbilitySystemComponent::UKavunAbilitySystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UKavunAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UKavunAbilitySystemComponent::TickComponent(float                        DeltaTime, ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
