

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomDecoration.generated.h"

class UActorSpawnerComponent;

UCLASS()
class PROJECTKAVUN_API ARoomDecoration : public AActor
{
	GENERATED_BODY()
	
public:	
	ARoomDecoration();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	TArray<TObjectPtr<UActorSpawnerComponent>> Spawners;
};
