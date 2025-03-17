// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GAS/BAbilitySystemTypes.h"
#include "BTTaskNode_PressAbilityInput.generated.h"

/**
 * 
 */
UCLASS()
class UBTTaskNode_PressAbilityInput : public UBTTaskNode
{
	GENERATED_BODY()
private:	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	UPROPERTY(EditAnywhere, Category = "GameplayAbility")
	EBAbilityInputID InputToPress;
};
