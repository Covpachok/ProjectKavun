// Copyright Covpachok

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "KavunAbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTKAVUN_API UKavunAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UKavunAbilitySystemComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float                        DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
