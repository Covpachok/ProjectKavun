// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorPoolComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(ActorPoolLog, Log, All);

UCLASS()
class PROJECTKAVUN_API UActorPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UActorPoolComponent();
	
protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "ActorPool")
	void Init();

	UFUNCTION(BlueprintCallable, Category = "ActorPool")
	void ChangeActorClass(TSubclassOf<AActor> NewActorClass);

	UFUNCTION(BlueprintCallable, Category = "ActorPool")
	bool Pull(AActor* & OutActor);

	UFUNCTION(BlueprintCallable, Category = "ActorPool")
	bool Push(AActor* Actor);

private:
	AActor* SpawnActor() const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "ActorPool", meta=(AllowPrivateAccess = true))
	TSubclassOf<AActor> ActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "ActorPool", meta=(AllowPrivateAccess = true, ClampMin = 1, UIMin = 1))
	int Size;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "ActorPool", meta=(AllowPrivateAccess = true))
	bool bCanExpand;

	UPROPERTY()
	TArray<AActor*> Pool;
	
	int LastIndex;
	int PulledCount;
};

UINTERFACE()
class UPoolActor : public UInterface
{
	GENERATED_BODY()
};

class PROJECTKAVUN_API IPoolActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="PoolActor")
	void OnPushed();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="PoolActor")
	void OnPulled(UActorPoolComponent* ActorPool);
};
