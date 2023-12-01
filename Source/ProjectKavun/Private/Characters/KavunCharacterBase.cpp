// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/KavunCharacterBase.h"

#include "Components/CapsuleComponent.h"
#include "Components/HealthComponent.h"

AKavunCharacterBase::AKavunCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	HealthComponent->OnHealthChanged.AddDynamic(this, &AKavunCharacterBase::OnHealthChanged);

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AKavunCharacterBase::OnHit);

	bReplicates = true;
}

void AKavunCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AKavunCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKavunCharacterBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                FVector              NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Display, TEXT("%s : Called"), __FUNCTIONW__);
}

void AKavunCharacterBase::OnHealthChanged(float CurrentHealth, float MaxHealth, float CurrentHealthChange,
                                          float MaxHealthChange)
{
	UE_LOG(LogTemp, Display, TEXT("%s : Health changed from %.2f/%.2f to %.2f/%.2f"), __FUNCTIONW__,
	       CurrentHealth - CurrentHealthChange, MaxHealth - MaxHealthChange, CurrentHealth, MaxHealth);

	if ( CurrentHealth <= 0 )
	{
		Die();
	}
}

void AKavunCharacterBase::TakeDamage(float Damage, const FVector& Impact, float KnockbackStrength)
{
	HealthComponent->TakeDamage(Damage);
}

void AKavunCharacterBase::Die()
{
	OnCharacterDeath.Broadcast(this);
}
