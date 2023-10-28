// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "CameraBlockerComponent.generated.h"

class AKavunCamera;

UENUM()
enum class ECameraBlockerType
{
	PosX,
	NegX,
	PosY,
	NegY,
};

/**
 * 
 */
UCLASS(meta=(BlueprintSpawnableComponent))
class PROJECTKAVUN_API UCameraBlockerComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UCameraBlockerComponent();

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32                OtherBodyIndex, bool         bFromSweep, const FHitResult&    SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                  int32                OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	ECameraBlockerType GetType() const { return Type; }

private:
	bool GetCameraFrom(AActor* Actor, AKavunCamera*& CameraRet);

private:
	UPROPERTY(EditAnywhere)
	ECameraBlockerType Type;
};
