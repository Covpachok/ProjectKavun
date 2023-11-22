#include "Components/EnemySpawnerComponent.h"

#include "Characters/EnemyCharacter.h"
#include "Components/BillboardComponent.h"

UEnemySpawnerComponent::UEnemySpawnerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	#if  WITH_EDITORONLY_DATA
	bVisualizeComponent = true;
	#endif
}

void UEnemySpawnerComponent::BeginPlay()
{
	Super::BeginPlay();

	#if  WITH_EDITORONLY_DATA
	if ( SpriteComponent )
	{
		SpriteComponent->SetSprite(LoadObject<UTexture2D>(
				nullptr, TEXT("/Game/Assets/Textures/T_Spawner.T_Spawner")));
	}
	#endif
}

void UEnemySpawnerComponent::TickComponent(float                        DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEnemySpawnerComponent::Spawn()
{
	if ( bEnemySpawned )
	{
		return;
	}

	UWorld* World = GetWorld();
	if ( !IsValid(World) )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : World is invalid??????"), __FUNCTIONW__);
		return;
	}

	if ( !IsValid(EnemyClass) )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : EnemyClass is invalid on %s."), __FUNCTIONW__, *GetName());
		return;
	}

	SpawnedEnemy = World->SpawnActor<AEnemyCharacter>(EnemyClass, GetComponentLocation(), GetComponentRotation());
	if ( !IsValid(SpawnedEnemy) )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : SpawnedEnemy of class %s is invalid on %s."), __FUNCTIONW__,
		       *EnemyClass->GetName(), *GetName());
		return;
	}

	SpawnedEnemy->OnCharacterDeath.AddDynamic(this, &UEnemySpawnerComponent::OnEnemyDied);
	bEnemySpawned = true;
}

void UEnemySpawnerComponent::Despawn()
{
	if ( !bEnemySpawned )
	{
		return;
	}

	if ( !IsValid(SpawnedEnemy) )
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : Trying to despawn SpawnedEnemy of class %s but it is invalid."),
		       __FUNCTIONW__, *EnemyClass->GetName(), *GetName());
	}
	else
	{
		SpawnedEnemy->Destroy();
	}

	bEnemySpawned = false;
}

void UEnemySpawnerComponent::OnEnemyDied(AKavunCharacterBase* Character)
{
	bEnemySpawned = false;
}
