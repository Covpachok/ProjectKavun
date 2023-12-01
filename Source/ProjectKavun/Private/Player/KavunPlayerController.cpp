#include "Player/KavunPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/PlayerCharacter.h"
#include "GameFramework/Character.h"

AKavunPlayerController::AKavunPlayerController()
{
	bReplicates = true;

	bShowMouseCursor = false;
	bAutoManageActiveCameraTarget = false;
}

void AKavunPlayerController::BeginPlay()
{
	Super::BeginPlay();

	checkf(DefaultMappingContext, TEXT("InputContext is invalid. Please set InputContext in %s."), *GetName());

	UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if ( Subsystem )
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("EnhancedInput success")));
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AKavunPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	check(EnhancedInputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AKavunPlayerController::Move);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AKavunPlayerController::Attack);
}

void AKavunPlayerController::Move(const FInputActionValue& Value)
{
	ACharacter* PlayerCharacter = GetCharacter();

	if ( !PlayerCharacter )
	{
		return;
	}

	const FVector2D MovementDirection = Value.Get<FVector2D>();
	PlayerCharacter->AddMovementInput(FVector::ForwardVector, MovementDirection.X);
	PlayerCharacter->AddMovementInput(FVector::RightVector, MovementDirection.Y);
}

void AKavunPlayerController::Attack(const FInputActionValue& Value)
{
	const FVector2D Direction = Value.Get<FVector2D>();

	if ( Direction.X >= 1 )
	{
		SetControlRotation({0, 0, 0});
	}
	else if ( Direction.X <= -1 )
	{
		SetControlRotation({0, 180, 0});
	}

	if ( Direction.Y >= 1 )
	{
		SetControlRotation({0, 90, 0});
	}
	else if ( Direction.Y <= -1 )
	{
		SetControlRotation({0, -90, 0});
	}
}
