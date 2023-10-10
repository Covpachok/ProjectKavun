// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Enemy.h"

#include "HealthComponent.h"
#include "Components/CapsuleComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule collider"));
	SetRootComponent(CapsuleComponent);
	CapsuleComponent->OnComponentHit.AddDynamic(this, &AEnemy::OnHit);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	HealthComponent->OnHealthChanged.AddDynamic(this, &AEnemy::OnHealthChanged);
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
	                                 FString::Printf(TEXT("Taken hit")));

	HealthComponent->TakeDamage(1);
}

void AEnemy::OnHit(UPrimitiveComponent* HitComponent,
                   AActor*              OtherActor,
                   UPrimitiveComponent* OtherComp,
                   FVector              NormalImpulse,
                   const FHitResult&    Hit)
{
	GEngine->AddOnScreenDebugMessage(-1,
	                                 5.f,
	                                 FColor::Green,
	                                 FString::Printf(TEXT("Taken hit")));

	if ( OtherActor->ActorHasTag(FName("PlayerProjectile")) )
	{
		HealthComponent->TakeDamage(1);
	}
}

void AEnemy::OnHealthChanged(float CurrentHealth, float MaxHealth)
{
	GEngine->AddOnScreenDebugMessage(-1,
	                                 5.f,
	                                 FColor::Green,
	                                 FString::Printf(TEXT("Health changed: %.2f/%.2f"), CurrentHealth, MaxHealth));

	if ( CurrentHealth == 0 )
	{
		Destroy();
	}
}
