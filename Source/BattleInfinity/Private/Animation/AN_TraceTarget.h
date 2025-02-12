// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AN_TraceTarget.generated.h"

/**
 * 
 */
UCLASS()
class UAN_TraceTarget : public UAnimNotify
{
	GENERATED_BODY()
public:	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(EditAnywhere, Category = "GameplayAbility")
	FGameplayTag TargetEventTag;

	UPROPERTY(EditAnywhere, Category = "GameplayAbility")
	bool bDrawDebug = true;

	UPROPERTY(EditAnywhere, Category = "GameplayAbility")
	bool bIgnoreOwner = true;

	UPROPERTY(EditAnywhere, Category = "GameplayAbility")
	float TraceRadius = 60.f;

	UPROPERTY(EditAnywhere, Category = "GameplayAbility")
	TArray<FName> TargetSocketNames;
};
