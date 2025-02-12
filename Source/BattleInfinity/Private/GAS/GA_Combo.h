// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/BGameplayAbility.h"
#include "GA_Combo.generated.h"

/**
 * 
 */
UCLASS()
class UGA_Combo : public UBGameplayAbility
{
	GENERATED_BODY()
public:	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	static FGameplayTag GetComboChangeTag();
	static FGameplayTag GetComboEndTag();
	static FGameplayTag GetComboTargetReceivedTag();

private:
	void BindInputPressDelegate();

	UFUNCTION()
	void HandleComboInputPress(float TimeWaited);

	void TryCommitNextCombo();

	UPROPERTY(EditDefaultsOnly, Category="GameplayEffect")
	TSubclassOf<UGameplayEffect> DefaultDamageEffect;

	UPROPERTY(EditDefaultsOnly, Category="Animation")
	UAnimMontage* ComboMontage;

	UFUNCTION()
	void HandleComboChange(FGameplayEventData Payload);

	UFUNCTION()
	void HandleDamageEvent(FGameplayEventData Payload);
	FName NextComboName;
};
