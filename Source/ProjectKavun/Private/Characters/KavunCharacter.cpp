// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectKavun/Public/Characters/KavunCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectiles/ProjectileBase.h"

AKavunCharacter::AKavunCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ShootingSpeed            = 5.f;
	ProjectileVelocityFactor = 35.f;
	ProjectileRangeFactor    = 50.f;

	ProjectileAngle      = 0.164;
	ProjectileDeltaAngle = 0;

	ProjectilesAmount = 6;
}

void AKavunCharacter::BeginPlay()
{
	Super::BeginPlay();

	if ( APlayerController* PlayerController = Cast<APlayerController>(Controller) )
	{
		if ( UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()) )
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AKavunCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKavunCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if ( UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent) )
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AKavunCharacter::Move);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AKavunCharacter::Shoot);
	}
}

void AKavunCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementDirection = Value.Get<FVector2D>();
	GEngine->AddOnScreenDebugMessage(0,
	                                 10.f,
	                                 FColor::Yellow,
	                                 FString::Printf(TEXT("Move: %s"), *MovementDirection.ToString()));

	if ( Controller )
	{
		AddMovementInput({1, 0, 0}, MovementDirection.X);
		AddMovementInput({0, 1, 0}, MovementDirection.Y);
	}
}

void AKavunCharacter::Shoot(const FInputActionValue& Value)
{
	FVector2D ShootDirection = Value.Get<FVector2D>();
	GEngine->AddOnScreenDebugMessage(1,
	                                 10.f,
	                                 FColor::Yellow,
	                                 FString::Printf(TEXT("Shoot: %s"), *ShootDirection.ToString()));

	if ( Controller )
	{
		if ( ShootDirection.X >= 1 )
			Controller->SetControlRotation({0, 0, 0});
		else if ( ShootDirection.X <= -1 )
			Controller->SetControlRotation({0, 180, 0});

		if ( ShootDirection.Y >= 1 )
			Controller->SetControlRotation({0, 90, 0});
		else if ( ShootDirection.Y <= -1 )
			Controller->SetControlRotation({0, -90, 0});
	}

	const float Time = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	if ( Time < LastTimeShoot + (1 - ShootingSpeed / 100) )
	{
		return;
	}
	LastTimeShoot = Time;

	SpawnProjectile();
}

void AKavunCharacter::SpawnProjectile()
{
	UWorld* const World = GetWorld();
	if ( ProjectileClass == nullptr || World == nullptr )
	{
		return;
	}

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	for ( int i = 0; i < ProjectilesAmount; ++i )
	{
		const FRotator SpawnRotation = {0.f, ProjectileDeltaAngle * (ProjectileAngle * 360.f), 0.f};
		const FVector  SpawnLocation = GetActorLocation();

		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		AProjectileBase* SpawnedProjectile = World->SpawnActor<AProjectileBase>(
				ProjectileClass,
				SpawnLocation,
				SpawnRotation,
				ActorSpawnParams);
		UProjectileMovementComponent* ProjectileMovementComponent = SpawnedProjectile->GetProjectileMovement();
		// ProjectileMovementComponent->AddForce(GetCharacterMovement()->Velocity * ProjectileVelocityFactor);
		ProjectileMovementComponent->ProjectileGravityScale = 1 - ProjectileRangeFactor / 100;

		++ProjectileDeltaAngle;
	}
	// GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("Spawned projectile, %s"), *GetCharacterMovement()->Velocity.ToString()));
}
