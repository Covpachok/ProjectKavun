


#include "Rooms/RoomDecorationBase.h"

// Sets default values
ARoomDecorationBase::ARoomDecorationBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARoomDecorationBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARoomDecorationBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

