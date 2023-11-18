// Fill out your copyright notice in the Description page of Project Settings.


#include "KavunAssetManager.h"

#include "AbilitySystemGlobals.h"

void UKavunAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	UAbilitySystemGlobals::Get().InitGlobalData();
	UE_LOG(LogTemp, Display, TEXT("%s : Loaded."), __FUNCTIONW__);
}
