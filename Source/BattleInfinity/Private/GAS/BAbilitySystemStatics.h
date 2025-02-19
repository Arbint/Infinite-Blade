// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "BAbilitySystemStatics.generated.h"

/**
 * 
 */
UCLASS()
class UBAbilitySystemStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:	
	static FGameplayTag GetDeathStatTag();
	static FGameplayTag GetHealthFullTag();
	static FGameplayTag GetHealthEmptyTag();
	static FGameplayTag GetManaFullTag();
	static FGameplayTag GetManaEmptyTag();
};
