// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BAIController.h"

#include "BehaviorTree/BlackboardComponent.h"

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
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABAIController::TargetPerceptionUpdated);
	}
}

void ABAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BehaviorTree);
}

void ABAIController::TargetPerceptionUpdated(AActor* TargetActor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		if (!GetCurrentTarget())
		{
			SetCurrentTarget(TargetActor);
		}
	}
	else
	{
		if (TargetActor == GetCurrentTarget())
		{
			SetCurrentTarget(GetNextPerceivedTarget());
		}
	}
}

const UObject* ABAIController::GetCurrentTarget() const
{
	if (!GetBlackboardComponent())
		return nullptr;
	
	return GetBlackboardComponent()->GetValueAsObject(TargetBlackboardKeyName);
}

void ABAIController::SetCurrentTarget(AActor* NewTarget)
{
	if (!GetBlackboardComponent())
		return;

	if (NewTarget)
		GetBlackboardComponent()->SetValueAsObject(TargetBlackboardKeyName, NewTarget);
	else
		GetBlackboardComponent()->ClearValue(TargetBlackboardKeyName);
}

AActor* ABAIController::GetNextPerceivedTarget() const
{
	TArray<AActor*> HostileActors;
	AIPerceptionComponent->GetPerceivedHostileActors(HostileActors);

	AActor* NextTarget = nullptr;
	float MinDistance = TNumericLimits<float>::Max();

	for (AActor* Target : HostileActors)
	{
		float DistanceToTarget = FVector::Distance(GetPawn()->GetActorLocation(), Target->GetActorLocation());
		if (DistanceToTarget < MinDistance)
		{
			MinDistance = DistanceToTarget;
			NextTarget = Target;
		}
	}

	return NextTarget;
}
