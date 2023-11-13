// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "KavunCamera.generated.h"

class APlayerCharacter;

UCLASS()
class PROJECTKAVUN_API AKavunCamera : public ACameraActor
{
	GENERATED_BODY()

public:
	AKavunCamera();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "Kavun Camera")
	void ChangeFollow(bool bNewFollow);

private:
	void Follow(float Delta);

	/* PUBLIC VARIABLES ARE BAD */
public:
	/* But I'm too lazy to write 4 get/setters */
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kavun Camera")
	bool bFollowPosX;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kavun Camera")
	bool bFollowPosY;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kavun Camera")
	bool bFollowNegX;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kavun Camera")
	bool bFollowNegY;

private:
	UPROPERTY(EditAnywhere)
	float FollowSpeed;

	TObjectPtr<APlayerCharacter> FollowCharacterRef;

	FVector LastFollowLocation;
	
	UPROPERTY(VisibleAnywhere)
	FVector tempdebugvariablepleasedeleteme;
};
