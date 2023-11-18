

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomDecorationBase.generated.h"

UCLASS()
class PROJECTKAVUN_API ARoomDecorationBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomDecorationBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
