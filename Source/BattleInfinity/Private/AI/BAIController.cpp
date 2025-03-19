// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BAIController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"

#include "GAS/BAbilitySystemStatics.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"

ABAIController::ABAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("Perception Component");

	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>("Damage Config");
	DamageConfig->SetMaxAge(5.f);
	AIPerceptionComponent->ConfigureSense(*DamageConfig);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight Config");
	SightConfig->SightRadius = 1000.f;
	SightConfig->LoseSightRadius = 1200.f;

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;

	SightConfig->PeripheralVisionAngleDegrees = 120.f;
	SightConfig->SetMaxAge(5.f);

	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABAIController::TargetPerceptionUpdated);
	AIPerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &ABAIController::TargetForgotten);
}

void ABAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BehaviorTree);
}

void ABAIController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);
	if (UAbilitySystemComponent* PawnASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(NewPawn))
	{
		PawnASC->RegisterGameplayTagEvent(UBAbilitySystemStatics::GetDeathStatTag())
			.AddUObject(this, &ABAIController::PawnDeadTagUpdated);
	}
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
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
		{
			if (TargetASC->HasMatchingGameplayTag(UBAbilitySystemStatics::GetDeathStatTag()))
			{
				ForgetTargetImmediately(TargetActor);
			}
		}
	}
}

void ABAIController::TargetForgotten(AActor* TargetActor)
{
	if (TargetActor == GetCurrentTarget())
	{
		SetCurrentTarget(GetNextPerceivedTarget());
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

void ABAIController::ForgetTargetImmediately(AActor* TargetToForget)
{
	for (UAIPerceptionComponent::FActorPerceptionContainer::TIterator iter = PerceptionComponent->GetPerceptualDataIterator(); iter; ++iter)
	{
		if (iter->Key != TargetToForget)
			continue;

		for (FAIStimulus& Stimuli : iter->Value.LastSensedStimuli)
		{
			Stimuli.SetStimulusAge(TNumericLimits<float>::Max());
		}
	}
}

void ABAIController::PawnDeadTagUpdated(const FGameplayTag GameplayTag, int32 NewCount)
{
	if (NewCount != 0)
	{
		GetBrainComponent()->StopLogic("Dead");
		AIPerceptionComponent->AgeStimuli(TNumericLimits<float>::Max());
		SetAllSensesEnabled(false);
		SetCurrentTarget(nullptr);
	}
	else
	{
		SetAllSensesEnabled(true);
		GetBrainComponent()->StartLogic();
	}
}

void ABAIController::SetAllSensesEnabled(bool bSensesEnabled)
{
	for (auto Iter = AIPerceptionComponent->GetSensesConfigIterator(); Iter; ++Iter)
	{
		AIPerceptionComponent->SetSenseEnabled((*Iter)->GetSenseImplementation(), bSensesEnabled);
	}
}
