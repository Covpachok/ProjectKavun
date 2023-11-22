#include "Components/ActorSpawnerComponent.h"

#include "Components/BillboardComponent.h"

UActorSpawnerComponent::UActorSpawnerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	#if  WITH_EDITORONLY_DATA
	bVisualizeComponent = true;
	#endif
}

void UActorSpawnerComponent::OnRegister()
{
	Super::OnRegister();

	#if  WITH_EDITORONLY_DATA
	if ( SpriteComponent )
	{
		SpriteComponent->SetSprite(LoadObject<UTexture2D>(
				nullptr, TEXT("/Game/Assets/Textures/T_Spawner.T_Spawner")));
	}
	#endif
}

void UActorSpawnerComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnActor();
}

void UActorSpawnerComponent::TickComponent(float                        DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UActorSpawnerComponent::SpawnActor()
{
	UWorld* World = GetWorld();
	if ( !IsValid(World) )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : World is invalid."), __FUNCTIONW__);
		return;
	}

	if ( !IsValid(ActorClass) )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : %s - ActorClass is invalid."), __FUNCTIONW__, *GetName());
		return;
	}

	SpawnedActor = World->SpawnActor<AActor>(ActorClass, GetComponentLocation(), GetComponentRotation());
	if ( !IsValid(SpawnedActor) )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : Actor wasn't spawned for some reason."), __FUNCTIONW__);
		return;
	}
}
