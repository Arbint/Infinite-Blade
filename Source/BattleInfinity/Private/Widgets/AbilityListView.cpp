// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/AbilityListView.h"

void UAbilityListView::ConfigureWithAbilities(const TArray<TSubclassOf<class UGameplayAbility>>& AblityClasses)
{
	for (const TSubclassOf<UGameplayAbility>& AbilityClass : AblityClasses)
	{
		AddItem(AbilityClass->GetDefaultObject());
	}
}
