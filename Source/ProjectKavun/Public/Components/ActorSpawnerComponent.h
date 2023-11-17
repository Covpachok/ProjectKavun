#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ActorSpawnerComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
/* Spawn */
class PROJECTKAVUN_API UActorSpawnerComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UActorSpawnerComponent();

	virtual void OnRegister() override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float                        DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Actor Spawner")
	void SpawnActor();

	UFUNCTION(BlueprintCallable, Category = "Actor Spawner")
	void DestroySpawnedActor();

	UFUNCTION(BlueprintGetter, Category = "Actor Spawner")
	AActor* GetSpawnedActor() const { return SpawnedActor; }

	UFUNCTION(BlueprintGetter, Category = "Actor Spawner")
	bool IsActorSpawned() const { return bActorSpawned; }

private:
	UPROPERTY(EditAnywhere)
	bool bSpawnAtStart;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ActorClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter = "GetSpawnedActor")
	TObjectPtr<AActor> SpawnedActor;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter = "IsActorSpawned")
	bool bActorSpawned;
};
