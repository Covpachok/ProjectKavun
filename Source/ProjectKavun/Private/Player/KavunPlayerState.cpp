#include "Player/KavunPlayerState.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/KavunAbilitySystemComponent.h"
#include "AbilitySystem/KavunAttributeSet.h"

AKavunPlayerState::AKavunPlayerState()
{
	NetUpdateFrequency = 100.f;

	AbilitySystemComponent = CreateDefaultSubobject<UKavunAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UKavunAttributeSet>(TEXT("AttributeSet"));
}
