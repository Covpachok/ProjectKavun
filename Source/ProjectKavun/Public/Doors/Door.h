// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class APlayerCharacter;
enum class ERoomType : uint8;
class UBoxComponent;

UCLASS()
class PROJECTKAVUN_API ADoor : public AActor
{
	GENERATED_BODY()

public:
	ADoor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Door")
	void SetDoorMesh(UStaticMesh* NewMesh);

	UFUNCTION()
	void SetType(ERoomType NewType) { DoorType = NewType; };

protected:
	void Open();
	void Close();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32                OtherBodyIndex, bool         bFromSweep, const FHitResult&    SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                  int32                OtherBodyIndex);

public:
	UFUNCTION()
	void OnRoomCleared();

	UFUNCTION()
	void OnPlayerEnteredRoom(APlayerCharacter* Player, bool bRoomClear);

protected:
	UPROPERTY(EditAnywhere, Category="Components")
	UStaticMeshComponent* DoorMesh;

	UPROPERTY(EditAnywhere, Category="Components")
	UStaticMeshComponent* DoorFrameMesh;

	UPROPERTY(EditAnywhere, Category="Components")
	UBoxComponent* BoxCollision;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Door")
	ERoomType DoorType;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Door")
	bool bClosed;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Door")
	int KeysNeeded;
};
