// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ValueGauge.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "AbilitySystemComponent.h"
#include "GAS/BAttributeSet.h"

void UValueGauge::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (ProgressBar)
	{
		ProgressBar->SetFillColorAndOpacity(ProgressBarFillColor);
	}
}

void UValueGauge::InitializeWithAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayAttribute& ValueAttribute, const FGameplayAttribute& MaxValueAttribute)
{
	if (!AbilitySystemComponent)
		return;

	bool bFound = false;
	float NewValue = AbilitySystemComponent->GetGameplayAttributeValue(ValueAttribute, bFound);
	float NewMaxValue = AbilitySystemComponent->GetGameplayAttributeValue(MaxValueAttribute, bFound);
	
	if (bFound)
	{
		SetValue(NewValue, NewMaxValue);
	}
}

void UValueGauge::SetValue(float NewValue, float NewMaxValue)
{
	if (NewMaxValue == 0)
		return;
	
	float Percent = NewValue / NewMaxValue;
	if (ProgressBar)
	{
		ProgressBar->SetPercent(Percent);
	}

	FNumberFormattingOptions FormatingOptions = FNumberFormattingOptions().SetMaximumFractionalDigits(0);
	ValueText->SetText(
		FText::Format(
			FTextFormat::FromString("{0}/{1}"),
			FText::AsNumber(NewValue, &FormatingOptions),
			FText::AsNumber(NewMaxValue, &FormatingOptions)
		)
	);
}
