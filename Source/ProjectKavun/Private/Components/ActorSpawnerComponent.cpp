#include "Components/ActorSpawnerComponent.h"

#include "Components/BillboardComponent.h"

UActorSpawnerComponent::UActorSpawnerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	#if  WITH_EDITORONLY_DATA
	bVisualizeComponent = true;
	#endif
}

void UActorSpawnerComponent::OnRegister()
{
	Super::OnRegister();

	#if  WITH_EDITORONLY_DATA
	if (SpriteComponent)
	{
		SpriteComponent->SetSprite(LoadObject<UTexture2D>(
			nullptr, TEXT("/Game/Assets/Textures/T_Spawner.T_Spawner")));
	}
	#endif
}

void UActorSpawnerComponent::BeginPlay()
{
	Super::BeginPlay();

	if ( bSpawnAtStart )
	{
		SpawnActor();
	}
}

void UActorSpawnerComponent::TickComponent(float                        DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UActorSpawnerComponent::SpawnActor()
{
	if ( !bActorSpawned )
	{
		UWorld* World = GetWorld();
		if ( !IsValid(World) )
		{
			UE_LOG(LogTemp, Error, TEXT("UActorSpawnerComponent::SpawnActor : World is invalid."));
			return;
		}
		
		if ( !IsValid(ActorClass) )
		{
			UE_LOG(LogTemp, Error, TEXT("UActorSpawnerComponent::SpawnActor : %s - ActorClass is invalid."), *GetName());
			return;
		}
		
		SpawnedActor = World->SpawnActor<AActor>(ActorClass, GetComponentLocation(), GetComponentRotation());
		if(!IsValid(SpawnedActor))
		{
			UE_LOG(LogTemp, Error, TEXT("UActorSpawnerComponent::SpawnActor : Actor wasn't spawned for some reason."));
			return;
		}

		bActorSpawned = true;
	}
}

void UActorSpawnerComponent::DestroySpawnedActor()
{
	if ( !IsValid(SpawnedActor) )
	{
		UE_LOG(LogTemp, Error, TEXT("UActorSpawnerComponent::DestroySpawnedActor : SpawnedActor is invalid."));
		return;
	}

	SpawnedActor->Destroy();
}
