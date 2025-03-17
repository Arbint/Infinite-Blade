// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode_PressAbilityInput.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

EBTNodeResult::Type UBTTaskNode_PressAbilityInput::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* OwningAIC = OwnerComp.GetAIOwner();
	if (!OwningAIC)
		return EBTNodeResult::Failed;

	if (UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwningAIC->GetPawn()))
	{
		OwnerASC->PressInputID((int32)InputToPress);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
