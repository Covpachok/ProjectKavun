#include "Components/PlayerDetector.h"

#include "Aliases.h"
#include "Characters/PlayerCharacter.h"

UPlayerDetector::UPlayerDetector()
{
	UPrimitiveComponent::SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UPrimitiveComponent::SetCollisionResponseToChannels(ECR_Ignore);
	UPrimitiveComponent::SetCollisionResponseToChannel(ECC_PLAYER_CHARACTER, ECR_Overlap);
	SetGenerateOverlapEvents(true);

	OnComponentBeginOverlap.AddDynamic(this, &UPlayerDetector::OnOverlapBegin);
	OnComponentEndOverlap.AddDynamic(this, &UPlayerDetector::OnOverlapEnd);
}

void UPlayerDetector::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32             OtherBodyIndex, bool bFromSweep,
                                     const FHitResult&    SweepResult)
{
	if ( !IsValid(OtherActor) )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : Actor is invalid."), __FUNCTIONW__);
		return;
	}

	DetectedPlayer = Cast<APlayerCharacter>(OtherActor);
	if ( !IsValid(DetectedPlayer) )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : Actor is not APlayerCharacter."), __FUNCTIONW__);
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("%s : %s"), __FUNCTIONW__, *OtherActor->GetName());
	bPlayerDetected = true;

	OnDetectorOverlapBegin.Broadcast(DetectedPlayer);
}

void UPlayerDetector::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32             OtherBodyIndex)
{
	if ( !IsValid(OtherActor) )
	{
		UE_LOG(LogTemp, Error, TEXT("%s : Actor is invalid."), __FUNCTIONW__);
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("%s : %s"), __FUNCTIONW__, *OtherActor->GetName());
	bPlayerDetected = false;

	OnDetectorOverlapEnd.Broadcast(DetectedPlayer);
}
