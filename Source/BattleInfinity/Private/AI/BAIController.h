// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameplayTagContainer.h"
#include "Perception/AIPerceptionTypes.h"
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
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* NewPawn) override;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Perception")
	class UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY()
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	UFUNCTION()
	void TargetPerceptionUpdated(AActor* TargetActor, FAIStimulus Stimulus);

	UFUNCTION()
	void TargetForgotten(AActor* TargetActor);

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TargetBlackboardKeyName = "Target";

	const UObject* GetCurrentTarget() const;

	void SetCurrentTarget(AActor* NewTarget);

	AActor* GetNextPerceivedTarget() const;

	void ForgetTargetImmediately(AActor* TargetToForget);

	void PawnDeadTagUpdated(const FGameplayTag GameplayTag, int32 NewCount);

	void SetAllSensesEnabled(bool bSensesEnabled);
};
