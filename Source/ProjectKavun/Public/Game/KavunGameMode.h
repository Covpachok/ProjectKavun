

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KavunGameMode.generated.h"

class UKavunGameInstance;
/**
 * 
 */
UCLASS()
class PROJECTKAVUN_API AKavunGameMode : public AGameModeBase
{
	GENERATED_BODY()


public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
	virtual void InitGameState() override;

	UFUNCTION(Exec, BlueprintCallable, Category = "Items")
	void GiveItem(const FName &ItemName);
	
private:
	TObjectPtr<UKavunGameInstance> GameInstance;
};
