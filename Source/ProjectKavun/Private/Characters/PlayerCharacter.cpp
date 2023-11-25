// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectKavun/Public/Characters/PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "Projectiles/Projectile.h"
#include "Weapons/WeaponComponent.h"
#include "Aliases.h"
#include "GameFramework/CharacterMovementComponent.h"

constexpr int KMaxMoney = 99;
constexpr int KMaxKeys  = 99;

APlayerCharacter::APlayerCharacter()
	: CurrentMoney(0),
	  CurrentKeys(0)
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectilePool  = CreateDefaultSubobject<UActorPoolComponent>(TEXT("ProjectilePool_TEST"));
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent_TEST"));

	CameraCollider = CreateDefaultSubobject<USphereComponent>(TEXT("CameraCollider"));
	CameraCollider->SetupAttachment(RootComponent);
	CameraCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CameraCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
	CameraCollider->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	CameraCollider->SetCollisionObjectType(ECC_Vehicle);
	CameraCollider->SetGenerateOverlapEvents(true);
	CameraCollider->SetSphereRadius(50.f);
	CameraCollider->SetRelativeLocation({0, 0, 1500});

	GetCapsuleComponent()->SetCollisionObjectType(ECC_PLAYER_CHARACTER);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_PLAYER_PROJECTILE, ECR_Ignore);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if ( const APlayerController* PlayerController = Cast<APlayerController>(Controller) )
	{
		if ( UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()) )
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if ( UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent) )
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Shoot);
	}
}

void APlayerCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                             FVector              NormalImpulse, const FHitResult& Hit)
{
	// UE_LOG(LogTemp, Warning, TEXT("%s : BEEP"), __FUNCTIONW__);
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementDirection = Value.Get<FVector2D>();
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

void APlayerCharacter::Shoot(const FInputActionValue& Value)
{
	const FVector2D ShootDirection = Value.Get<FVector2D>();
	GEngine->AddOnScreenDebugMessage(1,
	                                 10.f,
	                                 FColor::Yellow,
	                                 FString::Printf(TEXT("Shoot: %s"), *ShootDirection.ToString()));

	if ( Controller )
	{
		if ( ShootDirection.X >= 1 )
		{
			Controller->SetControlRotation({0, 0, 0});
		}
		else if ( ShootDirection.X <= -1 )
		{
			Controller->SetControlRotation({0, 180, 0});
		}

		if ( ShootDirection.Y >= 1 )
		{
			Controller->SetControlRotation({0, 90, 0});
		}
		else if ( ShootDirection.Y <= -1 )
		{
			Controller->SetControlRotation({0, -90, 0});
		}
	}

	if ( IsValid(WeaponComponent) )
	{
		WeaponComponent->Shoot(GetActorLocation(), Controller->GetControlRotation());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s : WeaponComponent is invalid."), __FUNCTIONW__);
	}
}

void APlayerCharacter::TakeDamage(float Damage, const FVector& Impact, float KnockbackStrength)
{
	Super::TakeDamage(Damage, Impact, KnockbackStrength);

	// GetCharacterMovement()->AddImpulse(Impact * KnockbackStrength * 1000);
	LaunchCharacter(Impact * KnockbackStrength * 100, true, false);
}

void APlayerCharacter::AddMoney(int Number)
{
	CurrentMoney = FMath::Clamp(CurrentMoney + Number, 0, KMaxMoney);
}

void APlayerCharacter::AddKeys(int Number)
{
	CurrentKeys = FMath::Clamp(CurrentKeys + Number, 0, KMaxKeys);
}

/*
void APlayerCharacter::SpawnProjectile()
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

		AActor* ProjectileActor = nullptr;
		// ProjectilePool->Pull(ProjectileActor);
		if ( ProjectileActor == nullptr )
		{
			return;
		}
		ProjectileActor->SetActorLocationAndRotation(SpawnLocation, SpawnRotation);

		UProjectileMovementComponent* ProjectileMovementComponent =
				ProjectileActor->GetComponentByClass<UProjectileMovementComponent>();

		if ( ProjectileMovementComponent == nullptr )
		{
			return;
		}

		ProjectileMovementComponent->Velocity = ProjectileActor->GetActorForwardVector() * ProjectileMovementComponent->
		                                        InitialSpeed;
		ProjectileMovementComponent->AddForce(GetCharacterMovement()->Velocity * ProjectileVelocityFactor);
		ProjectileMovementComponent->ProjectileGravityScale = 0;

		++ProjectileDeltaAngle;
		++ProjectilesSpawned;
	}
}
*/
