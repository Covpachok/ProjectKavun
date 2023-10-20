// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterAttributesComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterAttributesChangedCallbackSignature);


UENUM()
enum ECharacterAttributes : uint8
{
	ECharacterAttributes_MovementSpeed UMETA(DisplayName = "MovementSpeed"),
	ECharacterAttributes_Damage UMETA(DisplayName = "Damage"),
	ECharacterAttributes_DamageMultiplier UMETA(DisplayName = "DamageMultiplier"),
	ECharacterAttributes_ProjectilesPerSecond UMETA(DisplayName = "ProjectilesPerSecond"),
	ECharacterAttributes_ProjectilesPerSecondMultiplier UMETA(DisplayName = "ProjectilesPerSecondMultiplier"),
	ECharacterAttributes_ProjectileSpeed UMETA(DisplayName = "ProjectileSpeed"),
	ECharacterAttributes_ProjectileRange UMETA(DisplayName = "ProjectileRange"),
	ECharacterAttributes_Luck UMETA(DisplayName = "Luck"),
	ECharacterAttributes_Count UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FCharacterAttribute
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float Min = 0;

	UPROPERTY(EditAnywhere)
	float Max = 1;

	UPROPERTY(EditAnywhere)
	float Base = 1;

	UPROPERTY(VisibleAnywhere)
	bool bIsMultiplier = false;

	UPROPERTY(VisibleAnywhere)
	bool bHasMultiplier = false;

	UPROPERTY(VisibleAnywhere)
	TEnumAsByte<ECharacterAttributes> MultiplierSignature;

	UPROPERTY(VisibleAnywhere)
	float Current = 0;

	UPROPERTY(VisibleAnywhere)
	float TotalChange = 0;

	UPROPERTY(VisibleAnywhere)
	float LastChange = 0;
};


/**
 * Stores all player character stats
 */
UCLASS()
class PROJECTKAVUN_API UCharacterAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnCharacterAttributesChangedCallbackSignature OnCharacterAttributesChanged;

	UCharacterAttributesComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Character Attributes")
	void RecalculateAll();

	UFUNCTION(BlueprintCallable, Category = "Character Attributes")
	FString ToString() const;


	UFUNCTION(BlueprintCallable, Category = "Character Attributes")
	FORCEINLINE void SetAttributeBase(const ECharacterAttributes AttributeSignature, const float NewBase)
	{
		Attributes[AttributeSignature].Base = NewBase;
		RecalculateAttribute(AttributeSignature);
	}

	UFUNCTION(BlueprintCallable, Category = "Character Attributes")
	FORCEINLINE void ChangeAttribute(const ECharacterAttributes AttributeSignature, const float Change)
	{
		Attributes[AttributeSignature].TotalChange += Change;
		RecalculateAttribute(AttributeSignature);
	}


	UFUNCTION(BlueprintCallable, Category = "Character Attributes")
	FORCEINLINE float GetAttribute(const ECharacterAttributes AttributeSignature) const
	{
		return Attributes[AttributeSignature].Current;
	}

	UFUNCTION(BlueprintCallable, Category = "Character Attributes")
	FORCEINLINE float GetAttributeBase(const ECharacterAttributes AttributeSignature) const
	{
		return Attributes[AttributeSignature].Base;
	}

	UFUNCTION(BlueprintCallable, Category = "Character Attributes")
	FORCEINLINE float GetAttributeLastChange(const ECharacterAttributes AttributeSignature) const
	{
		return Attributes[AttributeSignature].LastChange;
	}

private:
	FORCEINLINE void RecalculateAttribute(const ECharacterAttributes AttributeSignature)
	{
		if ( Attributes[AttributeSignature].bHasMultiplier )
		{
			RecalculateAttributeWithMultiplier(AttributeSignature);
		}
		else
		{
			RecalculateAttributePlain(AttributeSignature);
		}
	}

	void RecalculateAttributePlain(const ECharacterAttributes AttributeSignature);

	void RecalculateAttributeWithMultiplier(const ECharacterAttributes AttributeSignature);

private:
	UPROPERTY(EditAnywhere, Category = "Character Attributes")
	FCharacterAttribute Attributes[ECharacterAttributes_Count];
};
