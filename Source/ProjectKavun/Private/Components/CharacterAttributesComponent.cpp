// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CharacterAttributesComponent.h"

UCharacterAttributesComponent::UCharacterAttributesComponent()
	: Attributes{
			/** Movement Speed */
			{0.25, 2, 1, false, false},

			/** Damage */
			{1, 5000, 3, false, true, ECharacterAttributes_DamageMultiplier},

			/** Damage Multiplier */
			{0.1, 10, 1, true, false},

			/** Projectiles Per Second */
			{0.25, 50, 3, false, true, ECharacterAttributes_ProjectilesPerSecondMultiplier},

			/** Projectiles Per Second Multiplier */
			{0.1, 10, 1, true, false},

			/** Projectile Speed */
			{50, 5000, 1000, false, false},

			/** Projectile Range */
			{50, 50000, 3000, false, false},

			/** Luck */
			{-100, 100, 1, false, false},
	}
{
}

void UCharacterAttributesComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCharacterAttributesComponent::RecalculateAll()
{
	for ( uint8 i = 0; i < ECharacterAttributes_Count; ++i )
	{
		/** Recalculating in reverse order, because Multipliers going right after the base values and base values needs multipliers */
		RecalculateAttribute(static_cast<ECharacterAttributes>(ECharacterAttributes_Count - i - 1));
	}
}

FString UCharacterAttributesComponent::ToString() const
{
	FString Result;

	const float MovementSpeed = Attributes[ECharacterAttributes_MovementSpeed].Current;
	Result += FString::Printf(TEXT("MSP: %02.2f\n"), MovementSpeed);

	const float Damage           = Attributes[ECharacterAttributes_Damage].Current;
	const float DamageMultiplier = Attributes[ECharacterAttributes_DamageMultiplier].Current;
	Result += FString::Printf(TEXT("DMG: %02.2f (%01.2f%%)\n"), Damage, DamageMultiplier);

	const float ProjectilesPerSecond           = Attributes[ECharacterAttributes_ProjectilesPerSecond].Current;
	const float ProjectilesPerSecondMultiplier = Attributes[ECharacterAttributes_ProjectilesPerSecondMultiplier].
			Current;
	Result += FString::Printf(TEXT("PPS: %02.2f (%01.2f%%)\n"), ProjectilesPerSecond, ProjectilesPerSecondMultiplier);

	const float ProjectileRange = Attributes[ECharacterAttributes_ProjectileRange].Current;
	Result += FString::Printf(TEXT("PRG: %.0f\n"), ProjectileRange);

	const float ProjectileSpeed = Attributes[ECharacterAttributes_ProjectileSpeed].Current;
	Result += FString::Printf(TEXT("PSP: %.0f\n"), ProjectileSpeed);

	return Result;
}

void UCharacterAttributesComponent::RecalculateAttributePlain(const ECharacterAttributes AttributeSignature)
{
	FCharacterAttribute& Attribute = Attributes[AttributeSignature];

	const float OldValue = Attribute.Current;

	Attribute.Current = FMath::Clamp(Attribute.Base + Attribute.TotalChange, Attribute.Min, Attribute.Max);

	Attribute.LastChange = Attribute.Current - OldValue;

	if ( Attribute.bIsMultiplier )
	{
		RecalculateAttributeWithMultiplier(Attribute.MultiplierSignature);
	}
	else
	{
		OnCharacterAttributesChanged.Broadcast();
	}
}

void UCharacterAttributesComponent::RecalculateAttributeWithMultiplier(const ECharacterAttributes AttributeSignature)
{
	FCharacterAttribute&       Attribute           = Attributes[AttributeSignature];
	const FCharacterAttribute& AttributeMultiplier = Attributes[Attribute.MultiplierSignature];

	const float OldValue = Attribute.Current;

	Attribute.Current = FMath::Clamp((Attribute.Base + Attribute.TotalChange) * AttributeMultiplier.Current,
	                                 Attribute.Min, Attribute.Max);

	Attribute.LastChange = Attribute.Current - OldValue;

	OnCharacterAttributesChanged.Broadcast();
}
