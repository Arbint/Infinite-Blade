// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ValueGauge.generated.h"

struct FGameplayAttribute;
struct FOnAttributeChangeData;
/**

 * 
 */
UCLASS()
class UValueGauge : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;
	void InitializeWithAbilitySystem(
		class UAbilitySystemComponent* AbilitySystemComponent, 
		const FGameplayAttribute& ValueAttribute, 
		const FGameplayAttribute& MaxValueAttribute);

private:	
	void SetValue(float NewValue, float NewMaxValue);

	UPROPERTY(meta=(BindWidget))
	class UProgressBar* ProgressBar;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* ValueText;

	UPROPERTY(EditAnywhere, Category = "Value Gauge")
	FLinearColor ProgressBarFillColor = FColor::Green;

	void ValueChanged(const FOnAttributeChangeData& ChangedData);
	void MaxValueChanged(const FOnAttributeChangeData& ChangedData);

	float CachedValue;
	float CachedMaxValue;
};
