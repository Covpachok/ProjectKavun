#include "Actors/InteractableActorBase.h"

#include "Characters/PlayerCharacter.h"

AInteractableActorBase::AInteractableActorBase()
{
	PrimaryActorTick.bCanEverTick = true;

	NonInteractableTime = 1.f;
	bInteractable       = false;
}

void AInteractableActorBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(NoInteractionTimerHandle, this, &AInteractableActorBase::AllowInteractions,
	                                NonInteractableTime, false);
}

void AInteractableActorBase::ResetNonInteractableTimer()
{
	GetWorldTimerManager().ClearTimer(NoInteractionTimerHandle);
	GetWorldTimerManager().SetTimer(NoInteractionTimerHandle, this, &AInteractableActorBase::AllowInteractions,
	                                NonInteractableTime, false);
}

void AInteractableActorBase::OnOverlapInteractable(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComp, int32             OtherBodyIndex,
                                                   bool                 bFromSweep,
                                                   const FHitResult&    SweepResult)
{
	if ( !IsValid(OtherActor) )
	{
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("%s : Overlapped with %s."), __FUNCTIONW__, *OtherActor->GetName());

	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if ( !IsValid(Player) )
	{
		return;
	}

	OnInteracted(Player);
}

void AInteractableActorBase::AllowInteractions()
{
	bInteractable = true;

	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, APlayerCharacter::StaticClass());

	if ( OverlappingActors.IsEmpty() )
	{
		return;
	}

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

		OnInteracted(Player);
		break;
	}
}

void AInteractableActorBase::OnInteracted_Implementation(APlayerCharacter* Player)
{
	if ( !bInteractable )
	{
		return;
	}

	/* Just in case */
	if ( !IsValid(Player) )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : Player is invalid."), __FUNCTIONW__);
		return;
	}
}
