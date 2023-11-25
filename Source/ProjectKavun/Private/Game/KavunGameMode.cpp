#include "Game/KavunGameMode.h"

#include "Characters/PlayerCharacter.h"
#include "Components/InventoryComponent.h"
#include "Game/KavunGameInstance.h"
#include "Items/ItemBase.h"
#include "Kismet/GameplayStatics.h"

void AKavunGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	GameInstance = Cast<UKavunGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

void AKavunGameMode::InitGameState()
{
	Super::InitGameState();
}

void AKavunGameMode::GiveItem(const FName& ItemName)
{
	FItemData ItemData;
	if ( !GameInstance->GetItemData(ItemName, ItemData) )
	{
		return;
	}

	if ( !IsValid(ItemData.ItemClass) )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : ItemClass on Item %s is invalid."), __FUNCTIONW__,
		       *ItemName.ToString());
		return;
	}

	UItemBase* Item = NewObject<UItemBase>(this, ItemData.ItemClass);
	if ( !IsValid(Item) )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : Item hasn't been created."), __FUNCTIONW__);
		return;
	}

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if ( !IsValid(PlayerCharacter) )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : PlayerCharacter is invalid."), __FUNCTIONW__);
		return;
	}

	UInventoryComponent* Inventory = PlayerCharacter->GetComponentByClass<UInventoryComponent>();
	if ( !IsValid(Inventory) )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : InventoryCompontent not found on %s."), __FUNCTIONW__,
		       *PlayerCharacter->GetName());
		return;
	}

	Inventory->AddItem(Item);
}
