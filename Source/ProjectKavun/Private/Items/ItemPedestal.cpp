#include "Items/ItemPedestal.h"

#include "Aliases.h"
#include "Components/CapsuleComponent.h"
#include "Components/InventoryComponent.h"
#include "Game/KavunGameInstance.h"
#include "Items/ItemDataAsset.h"
#include "Items/LootTable.h"
#include "Items/ItemBase.h"
#include "Items/LootTableManager.h"
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
			UE_LOG(LogTemp, Error, TEXT("%s : GameInstance is invalid for some reason lmao"), __FUNCTIONW__);
			return;
		}

		ULootTableManager* LootTableManager = GameInstance->GetLootTableManager();
		if ( !IsValid(LootTableManager) )
		{
			UE_LOG(LogTemp, Error, TEXT("%s : LootTableManager is invalid."), __FUNCTIONW__);
			return;
		}

		ULootTable* LootTable = GameInstance->GetLootTableManager()->GetLootTable(LootTableSource);
		if ( !IsValid(LootTable) )
		{
			UE_LOG(LogTemp, Error, TEXT("%s : LootTable of type[%d] is invalid"), __FUNCTIONW__,
			       static_cast<int>(LootTableSource));
			return;
		}

		bool bItemPicked = LootTable->PickRandomItem(ItemData);
		if ( !bItemPicked )
		{
			UE_LOG(LogTemp, Error, TEXT("%s : PickRandomItem returned false."), __FUNCTIONW__);
			return;
		}
	}
	else
	{
		FItemData* ItemDataPtr = ItemDataRow.GetRow<FItemData>("FItemData");
		if ( !ItemDataPtr )
		{
			UE_LOG(LogTemp, Error,
			       TEXT(
				       "AItemPedestal::BeginPlay : FItemData not found in DataTable. Maybe ItemDataRow is invalid or something."
			       ));
			return;
		}
		ItemData = *ItemDataPtr;
	}

	InitItem();
}

void AItemPedestal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItemPedestal::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32             OtherBodyIndex, bool bFromSweep,
                              const FHitResult&    SweepResult)
{
	if ( !IsValid(OtherActor) )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : OtherActor is invalid."), __FUNCTIONW__);
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("%s : Overlapped with %s."), __FUNCTIONW__, *OtherActor->GetName());

	if ( !bHasItem || !IsValid(ItemInstance) )
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : Has no item."), __FUNCTIONW__);
		return;
	}

	UInventoryComponent* InventoryComponent = OtherActor->GetComponentByClass<UInventoryComponent>();
	if ( !IsValid(InventoryComponent) )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : InventoryComponent is invalid."), __FUNCTIONW__);
		return;
	}

	ItemMesh->SetHiddenInGame(true);
	bHasItem = false;
	InventoryComponent->AddItem(ItemInstance);
}

void AItemPedestal::InitItem()
{
	// if ( !IsValid(ItemData) )
	// {
	// 	UE_LOG(LogTemp, Error,
	// 	       TEXT("%s : ItemData is invalid. Either forgot to set, or LootTable is fucked."), __FUNCTIONW__);
	// 	return;
	// }

	ItemMesh->SetStaticMesh(ItemData.StaticMesh);

	if ( !IsValid(ItemData.ItemClass) )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : ItemData.Item is invalid."), __FUNCTIONW__);
		return;
	}

	ItemInstance = NewObject<UItemBase>(this, ItemData.ItemClass);

	if ( !IsValid(ItemInstance) )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : ItemInstance is invalid."), __FUNCTIONW__);
		return;
	}

	ItemInstance->InitData();

	// if ( !IsValid(ItemInstance->GetData()) )
	// {
	// 	UE_LOG(LogTemp, Warning,
	// 	       TEXT("AItemPedestal::InitItem : ItemData of ItemInstance is invalid,"
	// 		       " YOU FORGOT TO SET IT, IDIOT. Nevermind, that's ok, I'll do that for you."));
	// 	ItemInstance->SetData(ItemData);
	// }

	bHasItem = true;
}
