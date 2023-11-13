


#include "Characters/EnemyCharacter.h"

#include "Aliases.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionObjectType(ECC_ENEMY_CHARACTER);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_ENEMY_PROJECTILE, ECR_Ignore);
}

void AEnemyCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Display, TEXT("AEnemyCharacter::OnHit : %s got hit by %s."), *GetName(), *OtherActor->GetName());
}

void AEnemyCharacter::OnHealthChanged(float CurrentHealth, float MaxHealth, float CurrentHealthChange,
		float MaxHealthChange)
{
	if(CurrentHealth == 0)
	{
		Destroy();
	}
}

void AEnemyCharacter::TakeDamage(float Damage, const FVector& Impact, float KnockbackStrength)
{
	Super::TakeDamage(Damage, Impact, KnockbackStrength);
	
	AController* CharacterController = GetController();
	if ( IsValid(CharacterController) && GetCharacterMovement()->IsMovingOnGround() )
	{
		CharacterController->StopMovement();
	}
	UE_LOG(LogTemp, Display, TEXT("ENEMY PUSHED BY %f in direction %s"), KnockbackStrength, *Impact.ToString());
	LaunchCharacter(Impact * KnockbackStrength * 100, true, false);
	// AddMovementInput(Impact, KnockbackStrength * 1000, true);
}
