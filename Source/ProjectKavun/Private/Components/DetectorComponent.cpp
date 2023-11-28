#include "Components/DetectorComponent.h"

#include "Aliases.h"
#include "Characters/EnemyCharacter.h"
#include "Characters/PlayerCharacter.h"

UDetectorComponent::UDetectorComponent()
{
	UPrimitiveComponent::SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UPrimitiveComponent::SetCollisionResponseToChannels(ECR_Ignore);
	UPrimitiveComponent::SetCollisionResponseToChannel(ECC_PLAYER_CHARACTER, ECR_Overlap);
	UPrimitiveComponent::SetCollisionResponseToChannel(ECC_ENEMY_CHARACTER, ECR_Overlap);
	SetGenerateOverlapEvents(true);

	OnComponentBeginOverlap.AddDynamic(this, &UDetectorComponent::OnOverlapBegin);
	OnComponentEndOverlap.AddDynamic(this, &UDetectorComponent::OnOverlapEnd);
}

void UDetectorComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UDetectorComponent::CheckForPlayer()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, APlayerCharacter::StaticClass());

	for ( auto& Actor : OverlappingActors )
	{
		if ( !IsValid(Actor) )
		{
			continue;
		}

		APlayerCharacter* Player = Cast<APlayerCharacter>(Actor);
		if ( !IsValid(Player) )
		{
			continue;
		}

		DetectedPlayer  = Player;
		bPlayerDetected = true;
		OnPlayerOverlapBegin.Broadcast(DetectedPlayer);
		UE_LOG(LogTemp, Display, TEXT("%s : Player detected."), __FUNCTIONW__);

		return true;
	}

	return false;
}

const TArray<AEnemyCharacter*>& UDetectorComponent::GetOverlappingEnemies()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, AEnemyCharacter::StaticClass());

	DetectedEnemies.Reset(OverlappingActors.Num());
	for ( auto& Actor : OverlappingActors )
	{
		if ( !Actor )
		{
			continue;
		}

		AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Actor);
		if ( !Enemy )
		{
			continue;
		}

		DetectedEnemies.Add(Enemy);
	}

	return DetectedEnemies;
}

void UDetectorComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                        const FHitResult&    SweepResult)
{
	if ( !IsValid(OtherActor) )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : Actor is invalid."), __FUNCTIONW__);
		return;
	}

	if ( !OtherActor->IsA(AKavunCharacterBase::StaticClass()) )
	{
		return;
	}

	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if ( IsValid(Player) )
	{
		UE_LOG(LogTemp, Display, TEXT("%s : Player detected."), __FUNCTIONW__);

		DetectedPlayer  = Player;
		bPlayerDetected = true;
		OnPlayerOverlapBegin.Broadcast(DetectedPlayer);

		return;
	}

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OtherActor);
	if ( IsValid(Enemy) )
	{
		UE_LOG(LogTemp, Display, TEXT("%s : Enemy detected."), __FUNCTIONW__);

		DetectedEnemies.Add(Enemy);
		OnEnemyOverlapBegin.Broadcast(Enemy);

		return;
	}
}

void UDetectorComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32             OtherBodyIndex)
{
	if ( !IsValid(OtherActor) )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : Actor is invalid."), __FUNCTIONW__);
		return;
	}

	if ( !OtherActor->IsA(AKavunCharacterBase::StaticClass()) )
	{
		return;
	}

	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if ( IsValid(Player) )
	{
		UE_LOG(LogTemp, Display, TEXT("%s : Player left detector."), __FUNCTIONW__);

		DetectedPlayer  = nullptr;
		bPlayerDetected = false;
		OnPlayerOverlapEnd.Broadcast(DetectedPlayer);

		return;
	}

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OtherActor);
	if ( IsValid(Enemy) )
	{
		UE_LOG(LogTemp, Display, TEXT("%s : Enemy left detector."), __FUNCTIONW__);

		DetectedEnemies.Remove(Enemy);
		OnEnemyOverlapEnd.Broadcast(Enemy);

		return;
	}
}
