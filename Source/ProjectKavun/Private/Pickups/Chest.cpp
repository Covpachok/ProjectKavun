#include "Pickups/Chest.h"

#include "Aliases.h"
#include "Characters/PlayerCharacter.h"
#include "Items/ItemPedestal.h"
#include "Pickups/PickupBase.h"

AChest::AChest()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComponent);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Collision->SetCollisionResponseToChannels(ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECC_PLAYER_CHARACTER, ECR_Overlap);
	Collision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	Collision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	Collision->SetGenerateOverlapEvents(true);
	Collision->SetSimulatePhysics(true);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AChest::OnOverlapInteractable);

	TrunkMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Trunk"));
	TrunkMesh->SetupAttachment(Collision);
	TrunkMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	TrunkMesh->SetCollisionResponseToChannels(ECR_Block);

	CoverMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cover"));
	CoverMesh->SetupAttachment(Collision);
	CoverMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	TrunkMesh->SetCollisionResponseToChannels(ECR_Block);

	bClosedOnKey = false;
	bOpened      = false;

	bCanSpawnItem   = false;
	ItemProbability = 0;

	MinPickupsDrop       = 0;
	MaxPickupsDrop       = 0;
	PickupsOverallWeight = 0;
}

void AChest::BeginPlay()
{
	Super::BeginPlay();

	CalculatePickupsOverallWeight();
}

void AChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChest::OnInteracted_Implementation(APlayerCharacter* Player)
{
	Super::OnInteracted_Implementation(Player);

	if ( !IsInteractable() || bOpened )
	{
		return;
	}

	if ( bClosedOnKey )
	{
		if ( Player->GetKeys() > 0 )
		{
			Player->SubtractKeys(1);
			bClosedOnKey = false;
			ResetNonInteractableTimer();
		}
		return;
	}

	if ( bCanSpawnItem && FMath::RandRange(0.f, 1.f) <= ItemProbability )
	{
		SpawnItemPedestal();
	}
	else
	{
		DropPickups();
	}

	OnOpened();
	bOpened = true;
}

void AChest::OnOpened_Implementation()
{
	UE_LOG(LogTemp, Display, TEXT("%s : %s opened."), __FUNCTIONW__, *GetName());
}

void AChest::CalculatePickupsOverallWeight()
{
	for ( auto& Data : PickupsDropData )
	{
		PickupsOverallWeight += Data.Weight;
	}
}

void AChest::SpawnItemPedestal()
{
	if ( !IsValid(ItemPedestalClass) )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : ItemPedestalClass is invalid."), __FUNCTIONW__);
		return;
	}

	UWorld* World = GetWorld();
	if ( !IsValid(World) )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : World is invalid."), __FUNCTIONW__);
		return;
	}

	ItemPedestalInstance = World->SpawnActor<AItemPedestal>(ItemPedestalClass, GetActorLocation(),
	                                                        FRotator::ZeroRotator);
	if ( !IsValid(ItemPedestalInstance) )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : Spawned ItemPedestalInstance is invalid."), __FUNCTIONW__);
		return;
	}

	Collision->SetSimulatePhysics(false);
	ItemPedestalInstance->OnItemPickedUp.AddDynamic(this, &AChest::OnItemPedestalUsed);
}

void AChest::DropPickups()
{
	if ( PickupsOverallWeight <= 0 || PickupsDropData.IsEmpty() )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : rly why"), __FUNCTIONW__);
		return;
	}

	const int DropCount = FMath::RandRange(MinPickupsDrop, MaxPickupsDrop);

	for ( int i = 0; i < DropCount; ++i )
	{
		const float ChosenWeight = FMath::RandRange(0.f, PickupsOverallWeight);

		TSubclassOf<APickupBase> PickupClass;
		float                    CurrentWeight = 0;
		for ( auto& Data : PickupsDropData )
		{
			CurrentWeight += Data.Weight;
			if ( CurrentWeight >= ChosenWeight )
			{
				PickupClass = Data.PickupClass;
				break;
			}
		}

		if ( !IsValid(PickupClass) )
		{
			UE_LOG(LogTemp, Error, TEXT("%s : PickupClass is invalid."), __FUNCTIONW__);
			continue;
		}

		UWorld* World = GetWorld();
		if ( !IsValid(World) )
		{
			UE_LOG(LogTemp, Error, TEXT("%s : World is invalid."), __FUNCTIONW__);
			continue;
		}

		FVector SpawnLocation = GetActorLocation();
		SpawnLocation.Z += 50;

		APickupBase* Pickup = World->SpawnActor<APickupBase>(PickupClass, SpawnLocation, FRotator::ZeroRotator);
		if ( !IsValid(Pickup) )
		{
			UE_LOG(LogTemp, Error, TEXT("%s : Spawned Pickup is invalid."), __FUNCTIONW__);
			continue;
		}

		UCapsuleComponent* PickupCollision = Pickup->GetCollision();
		if ( !IsValid(PickupCollision) )
		{
			UE_LOG(LogTemp, Error, TEXT("%s : PickupCollision is invalid."), __FUNCTIONW__);
			continue;
		}

		FVector Impulse = FMath::VRand();
		Impulse.Z       = 0.25f;

		PickupCollision->AddImpulse(Impulse);
	}
}

void AChest::OnItemPedestalUsed(const FItemData& ItemData)
{
	if ( IsValid(ItemPedestalInstance) )
	{
		ItemPedestalInstance->Destroy();
	}
	Collision->SetSimulatePhysics(true);
}
