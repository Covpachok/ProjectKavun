#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "EnemySpawnerComponent.generated.h"


class AKavunCharacterBase;
class AEnemyCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTKAVUN_API UEnemySpawnerComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UEnemySpawnerComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float                        DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Enemy Spawner")
	void Spawn();

	UFUNCTION(BlueprintCallable, Category = "Enemy Spawner")
	void Despawn();

	UFUNCTION(BlueprintGetter, Category = "Enemy Spawner")
	AEnemyCharacter* GetSpawnedEnemy() const { return SpawnedEnemy; }

	UFUNCTION(BlueprintGetter, Category = "Enemy Spawner")
	bool IsSpawned() const { return bEnemySpawned; }

private:
	UFUNCTION()
	void OnEnemyDied(AKavunCharacterBase *Character);

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemyCharacter> EnemyClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter = "GetSpawnedEnemy")
	TObjectPtr<AEnemyCharacter> SpawnedEnemy;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter = "IsSpawned")
	bool bEnemySpawned;
};
