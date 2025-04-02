// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/AbilityGauge.h"
#include "GAS/BGameplayAbility.h"
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
