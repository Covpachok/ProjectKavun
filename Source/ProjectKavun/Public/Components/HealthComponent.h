// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChangedDelegate, float, CurrentHealth, float, MaxHealth, float,
                                              CurrentHealthChange, float, MaxHealthChange);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTKAVUN_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnHealthChangedDelegate OnHealthChanged;

public:
	UHealthComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float                        DeltaTime,
	                           ELevelTick                   TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void TakeDamage(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Heal(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetMaxHealth(float NewValue);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetMaxHealthAndHeal(float NewValue);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void IncreaseMaxHealth(float Value);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetInvincibilityTime(float Value) { InvincibilityTimeAfterHit = Value; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = true))
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = true))
	float InvincibilityTimeAfterHit;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = true))
	float CurrentHealth;

	float HitTimer;
};
