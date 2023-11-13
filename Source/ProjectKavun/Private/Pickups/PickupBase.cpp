#include "Pickups/PickupBase.h"

#include "Characters/PlayerCharacter.h"

APickupBase::APickupBase()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(CollisionComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::OnOverlap);
}

void APickupBase::BeginPlay()
{
	Super::BeginPlay();
}

void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickupBase::OnPickedUp_Implementation(APlayerCharacter* Player)
{
	if ( !IsValid(Player) )
	{
		UE_LOG(LogTemp, Error, TEXT("APickupBase::OnPickedUp_Implementation : Player is invalid."));
		return;
	}

	// do stuff here
}

void APickupBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                            UPrimitiveComponent* OtherComp, int32             OtherBodyIndex, bool bFromSweep,
                            const FHitResult&    SweepResult)
{
	if ( !IsValid(OtherActor) )
	{
		UE_LOG(LogTemp, Error, TEXT("APickupBase::OnOverlap : OtherActor is invalid."));
		return;
	}

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if ( !IsValid(PlayerCharacter) )
	{
		UE_LOG(LogTemp, Error, TEXT("APickupBase::OnOverlap : OtherActor isn't the Player."));
		return;
	}

	OnPickedUp(PlayerCharacter);
}
