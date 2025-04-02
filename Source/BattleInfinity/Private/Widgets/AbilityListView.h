// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ListView.h"
#include "AbilityListView.generated.h"

/**
 * 
 */
UCLASS()
class UAbilityListView : public UListView
{
	GENERATED_BODY()
public:	
	void ConfigureWithAbilities(const TArray<TSubclassOf<class UGameplayAbility>>& AblityClasses);
};
