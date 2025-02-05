// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/OverheadStatWidget.h"
#include "Widgets/ValueGauge.h"
#include "GAS/BAttributeSet.h"


void UOverheadStatWidget::InitializeWithAbilitySystemComponent(UAbilitySystemComponent* AbilitySystemComponent)
{
	if (AbilitySystemComponent)
	{
		HealthBar->InitializeWithAbilitySystem(AbilitySystemComponent, UBAttributeSet::GetHealthAttribute(), UBAttributeSet::GetMaxHealthAttribute());
		ManaBar->InitializeWithAbilitySystem(AbilitySystemComponent, UBAttributeSet::GetManaAttribute(), UBAttributeSet::GetMaxManaAttribute());
	}
}
