// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameplayWidget.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Widgets/ValueGauge.h"
#include "Widgets/AbilityListView.h"
#include "GAS/BAttributeSet.h"


void UGameplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UAbilitySystemComponent* OwnerAbilitySystemComponent = 
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningPlayerPawn());

	if (OwnerAbilitySystemComponent)
	{
		HealthBar->InitializeWithAbilitySystem(OwnerAbilitySystemComponent, 
			UBAttributeSet::GetHealthAttribute(),
			UBAttributeSet::GetMaxHealthAttribute());

		ManaBar->InitializeWithAbilitySystem(OwnerAbilitySystemComponent,
			UBAttributeSet::GetManaAttribute(),
			UBAttributeSet::GetMaxManaAttribute());
	}
}

void UGameplayWidget::ConfigureWithAbilities(const TArray<TSubclassOf<class UGameplayAbility>>& AblityClasses)
{
	AbilityListView->ConfigureWithAbilities(AblityClasses);
}
