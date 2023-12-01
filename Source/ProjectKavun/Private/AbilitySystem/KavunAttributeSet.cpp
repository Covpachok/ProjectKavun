// Copyright Covpachok


#include "AbilitySystem/KavunAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

UKavunAttributeSet::UKavunAttributeSet()
{
	InitHealth(10.f);
	InitMaxHealth(10.f);
}

void UKavunAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UKavunAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKavunAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UKavunAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKavunAttributeSet, Health, OldHealth);
}

void UKavunAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKavunAttributeSet, MaxHealth, OldMaxHealth);
}
