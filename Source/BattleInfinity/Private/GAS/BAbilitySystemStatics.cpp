// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/BAbilitySystemStatics.h"

FGameplayTag UBAbilitySystemStatics::GetDeathStatTag()
{
	return FGameplayTag::RequestGameplayTag("stat.dead");
}

FGameplayTag UBAbilitySystemStatics::GetHealthFullTag()
{
	return FGameplayTag::RequestGameplayTag("stat.healthFull");
}

FGameplayTag UBAbilitySystemStatics::GetHealthEmptyTag()
{
	return FGameplayTag::RequestGameplayTag("stat.healthEmpty");
}

FGameplayTag UBAbilitySystemStatics::GetManaFullTag()
{
	return FGameplayTag::RequestGameplayTag("stat.manaFull");
}

FGameplayTag UBAbilitySystemStatics::GetManaEmptyTag()
{
	return FGameplayTag::RequestGameplayTag("stat.manaEmpty");
}
