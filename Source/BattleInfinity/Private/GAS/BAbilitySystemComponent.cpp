// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/BAbilitySystemComponent.h"
#include "GAS/BAttributeSet.h"
#include "GAS/BAbilitySystemStatics.h"

UBAbilitySystemComponent::UBAbilitySystemComponent()
{
	GetGameplayAttributeValueChangeDelegate(UBAttributeSet::GetHealthAttribute())
		.AddUObject(this, &UBAbilitySystemComponent::HealthUpdated);
}

void UBAbilitySystemComponent::ApplyInitialEffects()
{
	if (!GetOwner() || !GetOwner()->HasAuthority())
		return;

	for (const TSubclassOf<UGameplayEffect>& EffectClass : InitialEffects)
	{
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingSpec(EffectClass, 1, MakeEffectContext());
		ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
	}
}

void UBAbilitySystemComponent::GrantInitialAbilities()
{
	if (!GetOwner() || !GetOwner()->HasAuthority())
		return;

	for (const TPair<EBAbilityInputID, TSubclassOf<UGameplayAbility>>& AbilityPair : Abilities)
	{
		GiveAbility(FGameplayAbilitySpec(AbilityPair.Value, 0, (int32)AbilityPair.Key));
	}

	for (const TPair<EBAbilityInputID,TSubclassOf<UGameplayAbility>>& AbilityPair: BasicAbilities)
	{
		GiveAbility(FGameplayAbilitySpec(AbilityPair.Value, 4, (int32)AbilityPair.Key));
	}
}

void UBAbilitySystemComponent::HealthUpdated(const FOnAttributeChangeData& ChangeData)
{
	if (!GetOwner() || !GetOwner()->HasAuthority())
		return;

	bool bFound = false;
	float MaxHealth = GetGameplayAttributeValue(UBAttributeSet::GetMaxHealthAttribute(), bFound);
	if (ChangeData.NewValue >= MaxHealth)
	{
		if (!HasMatchingGameplayTag(UBAbilitySystemStatics::GetHealthFullTag()))
		{
			AddLooseGameplayTag(UBAbilitySystemStatics::GetHealthFullTag());
		}
	}
	else
	{
		RemoveLooseGameplayTag(UBAbilitySystemStatics::GetHealthFullTag());
	}

	if (ChangeData.NewValue == 0.f)
	{
		if (!HasMatchingGameplayTag(UBAbilitySystemStatics::GetHealthEmptyTag()))
		{
			AddLooseGameplayTag(UBAbilitySystemStatics::GetHealthEmptyTag());
			FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingSpec(DeathEffectClass, 1, MakeEffectContext());
			ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
		}
	}
	else
	{
		RemoveLooseGameplayTag(UBAbilitySystemStatics::GetHealthEmptyTag());
	}
}
