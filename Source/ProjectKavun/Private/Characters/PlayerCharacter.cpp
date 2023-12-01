// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectKavun/Public/Characters/PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "Projectiles/Projectile.h"
#include "Weapons/WeaponComponent.h"
#include "Aliases.h"
#include "KavunCamera.h"
#include "Components/DetectorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/KavunPlayerState.h"
#include "Rooms/RoomBase.h"

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

	DetectorOverlap();
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	CameraRef = GetWorld()->SpawnActor<AKavunCamera>(CameraClass);

	APlayerController* PlayerController = Cast<APlayerController>(NewController);
	PlayerController->SetViewTarget(CameraRef);
	CameraRef->SetFollowCharacter(this);

	InitAbilityActorInfo();
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                             FVector              NormalImpulse, const FHitResult& Hit)
{
	// UE_LOG(LogTemp, Warning, TEXT("%s : BEEP"), __FUNCTIONW__);
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
}

void APlayerCharacter::Shoot(const FInputActionValue& Value)
{
	if ( IsValid(WeaponComponent) )
	{
		WeaponComponent->Shoot(GetActorLocation(), Controller->GetControlRotation(), false);
		// for(int i = 0; i < 16; ++i)
		// {
		// 	UE_LOG(LogTemp, Warning, TEXT("BEEP"));
		// 	FRotator Rot = Controller->GetControlRotation();
		// 	Rot.Yaw += Rotations[i];
		// 	WeaponComponent->Shoot(GetActorLocation(), Rot, true);
		// }
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

void APlayerCharacter::InitAbilityActorInfo()
{
	AKavunPlayerState* State = GetPlayerState<AKavunPlayerState>();
	check(State);

	AbilitySystemComponent = State->GetAbilitySystemComponent();
	AttributeSet           = State->GetAttributeSet();

	AbilitySystemComponent->InitAbilityActorInfo(State, this);
}

void APlayerCharacter::DetectorOverlap()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, ARoomBase::StaticClass());
	for ( auto& Actor : OverlappingActors )
	{
		if ( !IsValid(Actor) )
		{
			continue;
		}

		ARoomBase* Room = Cast<ARoomBase>(Actor);
		if ( !IsValid(Room) )
		{
			continue;
		}

		TArray<UDetectorComponent*> Detectors;
		Room->GetComponents<UDetectorComponent>(Detectors);

		for ( auto& Detector : Detectors )
		{
			if ( !IsValid(Detector) )
			{
				continue;
			}

			FHitResult Dummy;
			Detector->OnOverlapBegin(nullptr, this, GetCapsuleComponent(), 0, false, Dummy);
		}
	}
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
