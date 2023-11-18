#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "PlayerDetector.generated.h"

class APlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDetectorDelegate, APlayerCharacter *, Player);

/**
 * Simple UBoxComponent wrapper 
 */
UCLASS(meta=(BlueprintSpawnableComponent))
class PROJECTKAVUN_API UPlayerDetector : public UBoxComponent
{
	GENERATED_BODY()

public:
	FDetectorDelegate OnDetectorOverlapBegin;
	FDetectorDelegate OnDetectorOverlapEnd;

public:
	UPlayerDetector();

	UFUNCTION(BlueprintCallable, Category = "Player Detector")
	APlayerCharacter* GetDetectedPlayer() const { return DetectedPlayer; }

	UFUNCTION(BlueprintCallable, Category = "Player Detector")
	bool IsPlayerDetected() const { return bPlayerDetected; }

private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32                OtherBodyIndex, bool         bFromSweep, const FHitResult&    SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                  int32                OtherBodyIndex);

private:
	TObjectPtr<APlayerCharacter> DetectedPlayer;
	bool                         bPlayerDetected;
};
