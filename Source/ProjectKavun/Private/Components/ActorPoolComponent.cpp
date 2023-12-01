// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ActorPoolComponent.h"

DEFINE_LOG_CATEGORY(ActorPoolLog);

UActorPoolComponent::UActorPoolComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	ActorClass = nullptr;
	Size       = 1;
	bCanExpand = false;
}

void UActorPoolComponent::BeginPlay()
{
	Super::BeginPlay();

	Init();
}

void UActorPoolComponent::Init()
{
	if ( !Pool.IsEmpty() )
	{
		for ( auto& Actor : Pool )
		{
			if ( IsValid(Actor) )
			{
				Actor->Destroy();
			}
		}
	}

	LastIndex = Size - 1;
	Pool.Init(nullptr, Size);

	for ( auto& Actor : Pool )
	{
		Actor = SpawnActor();
		if ( Actor )
		{
			IPoolActor::Execute_OnPushed(Actor);
		}
	}
}

void UActorPoolComponent::ChangeActorClass(TSubclassOf<AActor> NewActorClass)
{
	ActorClass = NewActorClass;
	Init();
}

bool UActorPoolComponent::Pull(AActor* & OutActor)
{
	if ( Pool.IsEmpty() )
	{
		UE_LOG(ActorPoolLog, Error, TEXT("%s : Pulling from an empty pool"), __FUNCTIONW__);
		return false;
	}

	if ( LastIndex == 0 )
	{
		if ( bCanExpand )
		{
			Push(SpawnActor());
		}
		else
		{
			UE_LOG(ActorPoolLog, Error, TEXT("%s : All actors in pool are used, can't pull"), __FUNCTIONW__);
			return false;
		}
	}

	if ( LastIndex >= Pool.Num() )
	{
		UE_LOG(ActorPoolLog, Warning, TEXT("%s : For some reason LastIndex >= PoolSize"), __FUNCTIONW__);
		LastIndex = Pool.Num() - 1;
	}


	AActor* Actor = Pool[0];
	Pool[0]       = nullptr;
	Swap(Pool[0], Pool[LastIndex]);
	--LastIndex;

	if ( !IsValid(Actor) )
	{
		UE_LOG(ActorPoolLog, Error, TEXT("%s : Pulled actor is invalid"), __FUNCTIONW__);
		return false;
	}

	OutActor = Actor;
	IPoolActor::Execute_OnPulled(Actor, this);
	++PulledCount;

	return true;
}

bool UActorPoolComponent::Push(AActor* Actor)
{
	if ( Actor->GetClass() != ActorClass )
	{
		return false;
	}

	if ( !IsValid(Actor) )
	{
		UE_LOG(ActorPoolLog, Error, TEXT("Attempted to push an invalid actor"))
		return false;
	}

	++LastIndex;
	if ( LastIndex >= Pool.Num() )
	{
		if ( bCanExpand )
		{
			Pool.Add(nullptr);
			LastIndex = Pool.Num() - 1;
		}
		else
		{
			UE_LOG(ActorPoolLog, Error, TEXT("Pool is full and can't expand"));
			LastIndex = Pool.Num() - 1;
			return false;
		}
	}

	AActor*& LastActor = Pool[LastIndex];
	if ( LastActor != nullptr )
	{
		UE_LOG(ActorPoolLog, Error, TEXT("For some reason an actor on the LastIndex isn't nullptr"));
		return false;
	}

	LastActor = Actor;
	IPoolActor::Execute_OnPushed(Actor);

	return true;
}

AActor* UActorPoolComponent::SpawnActor() const
{
	UWorld* const World = GetWorld();
	if ( !World )
	{
		UE_LOG(ActorPoolLog, Error, TEXT("GetWorld() returned nullptr"));
		return nullptr;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* NewActor = World->SpawnActor<AActor>(ActorClass, FVector::ZeroVector, FRotator::ZeroRotator,
	                                             SpawnParameters);
	if ( !IsValid(NewActor) )
	{
		UE_LOG(ActorPoolLog, Error, TEXT("World->SpawnActor() returned nullptr"));
		return nullptr;
	}

	return NewActor;
}
