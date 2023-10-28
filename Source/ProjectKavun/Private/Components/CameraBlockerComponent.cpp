// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CameraBlockerComponent.h"

#include "KavunCamera.h"
#include "Characters/KavunCharacter.h"

UCameraBlockerComponent::UCameraBlockerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	UPrimitiveComponent::SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UPrimitiveComponent::SetCollisionResponseToAllChannels(ECR_Ignore);
	UPrimitiveComponent::SetCollisionResponseToChannel(ECC_Vehicle, ECR_Overlap);
	UPrimitiveComponent::SetCollisionObjectType(ECC_WorldDynamic);
	SetGenerateOverlapEvents(true);

	SetLineThickness(2.f);
	ShapeColor = FColor::Orange;

	OnComponentBeginOverlap.AddDynamic(this, &UCameraBlockerComponent::OnOverlapBegin);
	OnComponentEndOverlap.AddDynamic(this, &UCameraBlockerComponent::OnOverlapEnd);
}

void UCameraBlockerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCameraBlockerComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                             const FHitResult&    SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow,
	                                 FString::Printf(TEXT("Camera blocker overlapped with: %s"),
	                                                 *OtherActor->GetName()));

	AKavunCamera* Camera = nullptr;
	if ( !GetCameraFrom(OtherActor, Camera) )
	{
		return;
	}

	switch ( Type )
	{
		case ECameraBlockerType::PosX:
			Camera->bFollowPosX = false;
			break;
		case ECameraBlockerType::NegX:
			Camera->bFollowNegX = false;
			break;
		case ECameraBlockerType::PosY:
			Camera->bFollowPosY = false;
			break;
		case ECameraBlockerType::NegY:
			Camera->bFollowNegY = false;
			break;
		default: ;
	}
}

void UCameraBlockerComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32             OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow,
	                                 FString::Printf(TEXT("camera blocker ended overlap with: %s"),
	                                                 *OtherActor->GetName()));

	AKavunCamera* Camera = nullptr;
	if ( !GetCameraFrom(OtherActor, Camera) )
	{
		return;
	}

	switch ( Type )
	{
		case ECameraBlockerType::PosX:
			Camera->bFollowPosX = true;
			break;
		case ECameraBlockerType::NegX:
			Camera->bFollowNegX = true;
			break;
		case ECameraBlockerType::PosY:
			Camera->bFollowPosY = true;
			break;
		case ECameraBlockerType::NegY:
			Camera->bFollowNegY = true;
			break;
		default: ;
	}
}

bool UCameraBlockerComponent::GetCameraFrom(AActor* Actor, AKavunCamera* & CameraRet)
{
	if ( !IsValid(Actor) )
	{
		return false;
	}

	AKavunCharacter* KavunCharacter = Cast<AKavunCharacter>(Actor);
	if ( !IsValid(KavunCharacter) )
	{
		return false;
	}

	CameraRet = KavunCharacter->GetCameraActor();
	if ( !IsValid(CameraRet) )
	{
		UE_LOG(LogTemp, Error, TEXT("UCameraBlockerComponent::GetCameraFrom : AKavunCamera is invalid."));
		return false;
	}

	return true;
}
