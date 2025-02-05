// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GAS/BAbilitySystemTypes.h"
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
	void GrantInitialAbilities();
	
private:	
	UPROPERTY(EditDefaultsOnly, Category = "GameplayEffects")
	TArray<TSubclassOf<UGameplayEffect>> InitialEffects;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TMap<EBAbilityInputID, TSubclassOf<UGameplayAbility>> Abilities;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TMap<EBAbilityInputID, TSubclassOf<UGameplayAbility>> BasicAbilities;
};
