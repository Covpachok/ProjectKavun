#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableActorBase.generated.h"

class APlayerCharacter;

UCLASS(Abstract)
class PROJECTKAVUN_API AInteractableActorBase : public AActor
{
	GENERATED_BODY()

public:
	AInteractableActorBase();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Interactable")
	void OnInteracted(APlayerCharacter* Player);

	UFUNCTION(BlueprintCallable, Category = "Interactable")
	bool IsInteractable() const { return bInteractable; }

	UFUNCTION(BlueprintCallable, Category = "Interactable")
	void ResetNonInteractableTimer();

protected:
	UFUNCTION()
	virtual void OnOverlapInteractable(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                   UPrimitiveComponent* OtherComp,
	                                   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void AllowInteractions();

private:
	UPROPERTY(EditAnywhere, Category = "Interactable")
	float NonInteractableTime;

	UPROPERTY(VisibleInstanceOnly, Category = "Interactable")
	bool bInteractable;

	FTimerHandle NoInteractionTimerHandle;
};
