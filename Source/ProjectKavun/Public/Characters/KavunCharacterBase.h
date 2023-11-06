// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KavunCharacterBase.generated.h"

class UHealthComponent;

UCLASS()
class PROJECTKAVUN_API AKavunCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AKavunCharacterBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float             GetMoveSpeed() const { return MoveSpeed; }
	UHealthComponent* GetHealthComponent() const { return HealthComponent; }

protected:
	UPROPERTY(EditAnywhere, Category = "KavunCharacter|Attributes")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, Category = "KavunCharacter|Attributes")
	UHealthComponent* HealthComponent;
};
