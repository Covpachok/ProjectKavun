// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectKavun/Public/Enemies/Enemy.h"

#include "Components/CapsuleComponent.h"
#include "Components/HealthComponent.h"
#include "Projectiles/Projectile.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	HealthComponent->OnHealthChanged.AddDynamic(this, &AEnemy::OnHealthChanged);

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AEnemy::OnHit);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if ( HealthComponent == nullptr )
	{
		GEngine->AddOnScreenDebugMessage(-1,
		                                 5.f,
		                                 FColor::Yellow,
		                                 FString::Printf(TEXT("Health component is null")));
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::TakeHit()
{
	GEngine->AddOnScreenDebugMessage(-1,
	                                 5.f,
	                                 FColor::Red,
	                                 FString::Printf(TEXT("AEnemy::TakeHit : Taken hit")));

	HealthComponent->TakeDamage(1);
}

void AEnemy::OnHit(UPrimitiveComponent* HitComponent,
                   AActor*              OtherActor,
                   UPrimitiveComponent* OtherComp,
                   FVector              NormalImpulse,
                   const FHitResult&    Hit)
{
	if ( OtherActor->GetClass() != GetClass() )
	{
		GEngine->AddOnScreenDebugMessage(-1,
		                                 5.f,
		                                 FColor::Green,
		                                 FString::Printf(TEXT("AEnemy::OnHit : Taken hit")));
	}

	AProjectile* Projectile = Cast<AProjectile>(OtherActor);
	if ( IsValid(Projectile) && OtherActor->ActorHasTag(FName("PlayerProjectile")) )
	{
		Projectile->HitEnemy(this);
		// HealthComponent->TakeDamage(Projectile->GetDamage());
	}
}

void AEnemy::OnHealthChanged(float CurrentHealth, float MaxHealth, float HealthChange, float MaxHealthChange)
{
	GEngine->AddOnScreenDebugMessage(-1,
	                                 5.f,
	                                 FColor::Green,
	                                 FString::Printf(
			                                 TEXT("Health changed: %02.2f/%02.2f - CH: %02.2f MH: %02.2f"),
			                                 CurrentHealth, MaxHealth, HealthChange, MaxHealthChange));

	if ( CurrentHealth == 0 )
	{
		Destroy();
	}
}
