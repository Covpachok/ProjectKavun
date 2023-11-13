#include "Items/ItemPedestal.h"

#include "Aliases.h"
#include "Components/CapsuleComponent.h"
#include "Components/InventoryComponent.h"
#include "Game/KavunGameInstance.h"
#include "Items/ItemDataAsset.h"
#include "Items/LootTable.h"
#include "Kismet/GameplayStatics.h"

AItemPedestal::AItemPedestal()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>("Collision");
	SetRootComponent(CapsuleCollision);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("Item Mesh");
	ItemMesh->SetupAttachment(RootComponent);
	ItemMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	PedestalMesh = CreateDefaultSubobject<UStaticMeshComponent>("Pedestal Mesh");
	PedestalMesh->SetupAttachment(RootComponent);
	PedestalMesh->SetCollisionResponseToAllChannels(ECR_Block);

	bTakeFromLootTable = false;
	bHasItem           = false;

	CapsuleCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CapsuleCollision->SetGenerateOverlapEvents(true);
	CapsuleCollision->SetCollisionObjectType(ECC_WorldDynamic);
	CapsuleCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	CapsuleCollision->SetCollisionResponseToChannel(ECC_PLAYER_CHARACTER, ECR_Overlap);

	CapsuleCollision->OnComponentBeginOverlap.AddDynamic(this, &AItemPedestal::OnOverlap);
}

void AItemPedestal::BeginPlay()
{
	Super::BeginPlay();


	if ( bTakeFromLootTable )
	{
		UKavunGameInstance* GameInstance = Cast<UKavunGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if ( !IsValid(GameInstance) )
		{
			UE_LOG(LogTemp, Error, TEXT("AItemPedestal::BeginPlay : GameInstance is invalid for some reason lmao"));
			return;
		}

		ULootTable* LootTable = GameInstance->GetLootTable(LootTableSource);
		if ( !IsValid(LootTable) )
		{
			UE_LOG(LogTemp, Error, TEXT("AItemPedestal::BeginPlay : LootTable of type[%d] is invalid"),
			       static_cast<int>(LootTableSource));
			return;
		}

		ItemData = LootTable->PickRandomItem();
	}

	InitItem();
}

void AItemPedestal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UItemDataAsset* AItemPedestal::TakeItemData()
{
	ItemMesh->SetStaticMesh(nullptr);
	return ItemData;
}

void AItemPedestal::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32             OtherBodyIndex, bool bFromSweep,
                              const FHitResult&    SweepResult)
{
	if ( !IsValid(OtherActor) )
	{
		UE_LOG(LogTemp, Error, TEXT("AItemPedestal::OnOverlap : OtherActor is invalid."));
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("AItemPedestal::OnOverlap : Overlapped with %s."), *OtherActor->GetName());

	if ( !bHasItem || !IsValid(ItemInstance) )
	{
		UE_LOG(LogTemp, Warning, TEXT("AItemPedestal::OnOverlap : Has no item."));
		return;
	}

	UInventoryComponent* InventoryComponent = OtherActor->GetComponentByClass<UInventoryComponent>();
	if ( !IsValid(InventoryComponent) )
	{
		UE_LOG(LogTemp, Error, TEXT("AItemPedestal::OnOverlap : InventoryComponent is invalid."));
		return;
	}

	InventoryComponent->AddItem(ItemInstance);
}

void AItemPedestal::InitItem()
{
	// Function with shitton of IsValid checks, cuz I hate when unreal crashes

	if ( !IsValid(ItemData) )
	{
		UE_LOG(LogTemp, Error,
		       TEXT("AItemPedestal::InitItem : ItemData is invalid. Either forgot to set, or LootTable is fucked."));
		return;
	}

	ItemMesh->SetStaticMesh(ItemData->StaticMesh);


	if ( !IsValid(ItemData->Item) )
	{
		UE_LOG(LogTemp, Error, TEXT("AItemPedestal::InitItem : ItemData->Item is invalid."));
		return;
	}

	ItemInstance = NewObject<UItemBase>(this, ItemData->Item);

	if ( !IsValid(ItemInstance) )
	{
		UE_LOG(LogTemp, Error, TEXT("AItemPedestal::InitItem : ItemInstance is invalid. WHY"));
		return;
	}

	if ( !IsValid(ItemInstance->GetData()) )
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("AItemPedestal::InitItem : ItemData of ItemInstance is invalid,"
			       " YOU FORGOT TO SET IT, IDIOT. Nevermind, that's ok, I'll do that for you."));
		ItemInstance->SetData(ItemData);
	}

	bHasItem = true;
}
