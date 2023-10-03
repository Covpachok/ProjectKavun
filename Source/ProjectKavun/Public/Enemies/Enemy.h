// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Enemy.generated.h"


class UCapsuleComponent;
class UHealthComponent;

UCLASS()
class PROJECTKAVUN_API AEnemy : public APawn
{
	GENERATED_BODY()

public:
	AEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void TakeHit();

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent,
	                   AActor*              OtherActor,
	                   UPrimitiveComponent* OtherComp,
	                   FVector              NormalImpulse,
	                   const FHitResult&    Hit);
	                   
	UFUNCTION()
	virtual void OnHealthChanged(float CurrentHealth, float MaxHealth);
private:
	UPROPERTY(EditAnywhere)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY()
	UHealthComponent* HealthComponent;
};
