// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "KavunCharacterBase.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
class UHealthComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterDeathDelegate, AKavunCharacterBase*, Character);

UCLASS()
class PROJECTKAVUN_API AKavunCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	FOnCharacterDeathDelegate OnCharacterDeath;

	AKavunCharacterBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Kavun Character")
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	                   UPrimitiveComponent* OtherComp, FVector    NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	virtual void OnHealthChanged(float CurrentHealth, float MaxHealth, float CurrentHealthChange,
	                             float MaxHealthChange);

	UFUNCTION(BlueprintCallable, Category = "Kavun Character")
	virtual void TakeDamage(float Damage, const FVector& Impact, float KnockbackStrength);

	UFUNCTION(BlueprintCallable, Category = "Kavun Character")
	virtual void Die();

	UHealthComponent* GetHealthComponent() const { return HealthComponent; }


	/**
	 * Ability System
	 * */

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	UAttributeSet*                   GetAttributeSet() const { return AttributeSet; }

protected:
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> BodyMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
};
