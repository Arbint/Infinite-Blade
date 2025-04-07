// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/AbilityGauge.h"
#include "GAS/BGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "Components/Image.h"

void UAbilityGauge::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	UBGameplayAbility* AbilityCDO = Cast<UBGameplayAbility>(ListItemObject);
	if (AbilityCDO)
	{
		Icon->GetDynamicMaterial()->SetTextureParameterValue("Icon", AbilityCDO->GetAbilityIcon());
	}
}

void UAbilityGauge::NativeConstruct()
{
	Super::NativeConstruct();
	if (UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningPlayerPawn()))
	{
		OwnerASC->AbilityCommittedCallbacks.AddUObject(this, &UAbilityGauge::AbilityCommited);
	}
}

void UAbilityGauge::AbilityCommited(UGameplayAbility* Ability)
{
	float CooldownDuration, CooldownTimeRemaining;
	Ability->GetCooldownTimeRemainingAndDuration(Ability->GetCurrentAbilitySpecHandle(),
		Ability->GetCurrentActorInfo(), CooldownTimeRemaining, CooldownDuration);
}
