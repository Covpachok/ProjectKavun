#include "Rooms/RoomDecorationBase.h"

#include "Characters/EnemyCharacter.h"
#include "Components/ActorSpawnerComponent.h"
#include "Components/EnemySpawnerComponent.h"

ARoomDecorationBase::ARoomDecorationBase()
{
	PrimaryActorTick.bCanEverTick = true;

	EnemiesCount    = 0;
	EnemyDeathCount = 0;
}

void ARoomDecorationBase::BeginPlay()
{
	Super::BeginPlay();

	GetComponents<UEnemySpawnerComponent>(EnemySpawners);
	if ( EnemySpawners.IsEmpty() )
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : Not found any UEnemySpawnerComponent on %s."), __FUNCTIONW__, *GetName());
	}


	GetComponents<UActorSpawnerComponent>(ActorSpawners);
	if ( ActorSpawners.IsEmpty() )
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : Not found any UActorSpawnerComponent on %s."), __FUNCTIONW__, *GetName());
	}
}

void ARoomDecorationBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARoomDecorationBase::ActivateSpawners()
{
	for ( auto& Spawner : EnemySpawners )
	{
		if ( !IsValid(Spawner) )
		{
			UE_LOG(LogTemp, Error, TEXT("%s : UEnemySpawner on %s is invalid."), __FUNCTIONW__, *GetName());
			continue;
		}

		Spawner->Spawn();

		if ( !Spawner->IsSpawned() )
		{
			continue;
		}

		AEnemyCharacter* Enemy = Spawner->GetSpawnedEnemy();
		if ( !IsValid(Enemy) )
		{
			UE_LOG(LogTemp, Error, TEXT("%s : Uh oh.. Recently spawned character is invalid."), __FUNCTIONW__);
			continue;
		}

		++EnemiesCount;
		Enemy->OnCharacterDeath.AddDynamic(this, &ARoomDecorationBase::CountEnemyDeaths);
	}
}

void ARoomDecorationBase::DeactivateSpawners()
{
	for ( auto& Spawner : EnemySpawners )
	{
		if ( !IsValid(Spawner) )
		{
			UE_LOG(LogTemp, Error, TEXT("%s : UEnemySpawner on %s is invalid."), __FUNCTIONW__, *GetName());
			continue;
		}

		Spawner->Despawn();
	}
	EnemiesCount = 0;
}

void ARoomDecorationBase::CountEnemyDeaths(AKavunCharacterBase* Character)
{
	++EnemyDeathCount;

	if ( EnemyDeathCount == EnemiesCount )
	{
	}
}
