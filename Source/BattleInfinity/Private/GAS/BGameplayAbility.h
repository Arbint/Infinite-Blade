// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class UBGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:			
	UBGameplayAbility();
	FORCEINLINE UTexture2D* GetAbilityIcon() const { return AbilityIcon; }

protected:
	UAnimInstance* GetOwnerAnimInstance() const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Visual")
	UTexture2D* AbilityIcon;
};
