// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, CurrentHealth, float, MaxHealth);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTKAVUN_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnHealthChangedSignature OnHealthChanged;

public:
	UHealthComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float                        DeltaTime,
	                           ELevelTick                   TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void TakeDamage(float Amount);
	void Heal(float Amount);

	void SetMaxHealth(float NewValue);
	void SetMaxHealthAndHeal(float NewValue);

private:
	UPROPERTY(VisibleAnywhere, Category = Health)
	float CurrentHealth;

	UPROPERTY(EditAnywhere, Category = Health)
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, Category = Health)
	float InvincibilityTimeAfterHit;

	float HitTimer;
};
