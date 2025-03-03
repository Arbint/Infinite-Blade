// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ABAIController::ABAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("Perception Component");
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight Config");

	if (SightConfig && AIPerceptionComponent)
	{
		SightConfig->SightRadius = 1000.f;
		SightConfig->LoseSightRadius = 1200.f;

		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = false;

		SightConfig->PeripheralVisionAngleDegrees = 120.f;

		AIPerceptionComponent->ConfigureSense(*SightConfig);
	}
}
