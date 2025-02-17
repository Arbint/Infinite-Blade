// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/BAbilitySystemComponent.h"
#include "GAS/BAttributeSet.h"

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
	if (ChangeData.NewValue == 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("I am Dead"))
	}
}
