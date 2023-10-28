// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "KavunCamera.generated.h"

class AKavunCharacter;
/**
 * 
 */
UCLASS()
class PROJECTKAVUN_API AKavunCamera : public ACameraActor
{
	GENERATED_BODY()

public:
	AKavunCamera();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

private:
	void Follow(float Delta);

public:
	/** I know, I know, public variables is bad
	 * but I don't want to write 8 boilerplate functions */
	
	UPROPERTY()
	bool bFollowPosX;
	UPROPERTY()
	bool bFollowPosY;
	UPROPERTY()
	bool bFollowNegX;
	UPROPERTY()
	bool bFollowNegY;

private:
	UPROPERTY(EditAnywhere)
	float FollowSpeed;

	UPROPERTY()
	AKavunCharacter* FollowCharacterRef;

	FVector LastFollowLocation;
};
