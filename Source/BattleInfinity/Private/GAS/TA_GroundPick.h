// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "TA_GroundPick.generated.h"

/**
 * 
 */
UCLASS()
class ATA_GroundPick : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
public:		
	ATA_GroundPick();
	virtual void Tick(float DeltaSeconds) override;

private:
	FVector GetTargetingAimLoc() const;

	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	float TargetMaxDistance = 2000.f;
};
