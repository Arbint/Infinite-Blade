// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BAIController.generated.h"

/**
 * 
 */
UCLASS()
class ABAIController : public AAIController
{
	GENERATED_BODY()
public:
	ABAIController();

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Perception")
	class UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY()
	class UAISenseConfig_Sight* SightConfig;
};
