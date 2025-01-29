// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "BAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class UBAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
private:
	UPROPERTY()
	FGameplayAttributeData Health;

	UPROPERTY()
	FGameplayAttributeData MaxHealth;
};
