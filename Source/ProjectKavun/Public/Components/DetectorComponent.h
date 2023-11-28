#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "DetectorComponent.generated.h"

class AInteractableActorBase;
class AKavunCharacterBase;
class AEnemyCharacter;
class APlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerDetectorDelegate, APlayerCharacter*, Player);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnemyDetectorDelegate, AEnemyCharacter*, Enemy);

/**
 * Simple UBoxComponent wrapper 
 */
UCLASS(meta=(BlueprintSpawnableComponent))
class PROJECTKAVUN_API UDetectorComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Detector")
	FPlayerDetectorDelegate OnPlayerOverlapBegin;
	UPROPERTY(BlueprintAssignable, Category = "Detector")
	FPlayerDetectorDelegate OnPlayerOverlapEnd;

	UPROPERTY(BlueprintAssignable, Category = "Detector")
	FEnemyDetectorDelegate OnEnemyOverlapBegin;
	UPROPERTY(BlueprintAssignable, Category = "Detector")
	FEnemyDetectorDelegate OnEnemyOverlapEnd;

public:
	UDetectorComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Detector")
	APlayerCharacter* GetDetectedPlayer() const { return DetectedPlayer; }

	UFUNCTION(BlueprintCallable, Category = "Detector")
	bool IsPlayerDetected() const { return bPlayerDetected; }

	UFUNCTION(BlueprintCallable, Category = "Detector")
	const TArray<AEnemyCharacter*>& GetDetectedEnemies() const { return DetectedEnemies; }

	UFUNCTION(BlueprintCallable, Category = "Detector")
	bool IsEnemiesDetected() const { return DetectedEnemies.Num() > 0; }

	UFUNCTION(BlueprintCallable, Category = "Detector")
	bool CheckForPlayer();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32                OtherBodyIndex, bool         bFromSweep, const FHitResult&    SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                  int32                OtherBodyIndex);

private:
	// UFUNCTION(BlueprintCallable, Category = "Detector")
	const TArray<AEnemyCharacter*>& GetOverlappingEnemies();

private:
	UPROPERTY()
	TObjectPtr<APlayerCharacter> DetectedPlayer;
	bool                         bPlayerDetected;

	UPROPERTY()
	TArray<AEnemyCharacter*> DetectedEnemies;
};
