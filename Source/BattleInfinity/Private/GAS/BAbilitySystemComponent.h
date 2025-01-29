// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class UBAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void ApplyInitialEffects();
	
private:	
	UPROPERTY(EditDefaultsOnly, Category = "GameplayEffects")
	TArray<TSubclassOf<UGameplayEffect>> InitialEffects;
};
