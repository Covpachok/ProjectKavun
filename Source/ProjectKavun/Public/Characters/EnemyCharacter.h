#pragma once

#include "CoreMinimal.h"
#include "Characters/KavunCharacterBase.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKAVUN_API AEnemyCharacter : public AKavunCharacterBase
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	                   UPrimitiveComponent* OtherComp, FVector    NormalImpulse, const FHitResult& Hit) override;

	virtual void OnHealthChanged(float CurrentHealth, float MaxHealth, float CurrentHealthChange, float MaxHealthChange) override;

	virtual void TakeDamage(float Damage, const FVector& Impact, float KnockbackStrength) override;
};
