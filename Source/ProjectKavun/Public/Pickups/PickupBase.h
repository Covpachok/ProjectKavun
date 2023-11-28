#pragma once

#include "CoreMinimal.h"
#include "Actors/InteractableActorBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "PickupBase.generated.h"

class APlayerCharacter;

UCLASS(Abstract)
class PROJECTKAVUN_API APickupBase : public AInteractableActorBase
{
	GENERATED_BODY()

public:
	APickupBase();

	UFUNCTION(BlueprintGetter, Category = "Pickup")
	UCapsuleComponent* GetCollision() const { return CollisionComponent; }

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintGetter = "GetCollision", Category = "Components")
	TObjectPtr<UCapsuleComponent> CollisionComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComponent;
};
