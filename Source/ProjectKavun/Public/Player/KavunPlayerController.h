#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "KavunPlayerController.generated.h"

class AKavunCamera;
class APlayerCharacter;

UCLASS()
class PROJECTKAVUN_API AKavunPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AKavunPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void Move(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input|Actions")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input|Actions")
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, Category = "Input|Actions")
	TObjectPtr<UInputAction> ActivateAction;

	UPROPERTY(EditAnywhere, Category = "Input|Actions")
	TObjectPtr<UInputAction> EnterAction;

	UPROPERTY(EditAnywhere, Category = "Input|Actions")
	TObjectPtr<UInputAction> ControlAction;
};
