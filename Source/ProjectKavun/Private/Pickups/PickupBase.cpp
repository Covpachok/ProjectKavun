#include "Pickups/PickupBase.h"

#include "Aliases.h"
#include "Characters/PlayerCharacter.h"

APickupBase::APickupBase()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComponent->SetCollisionResponseToChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_PLAYER_CHARACTER, ECR_Overlap);
	CollisionComponent->SetSimulatePhysics(true);

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::OnOverlapInteractable);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(CollisionComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetCollisionResponseToChannels(ECR_Ignore);
	MeshComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	MeshComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
}

void APickupBase::BeginPlay()
{
	Super::BeginPlay();
}
