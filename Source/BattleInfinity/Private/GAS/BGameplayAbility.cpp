// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/BGameplayAbility.h"
#include "Components/SkeletalMeshComponent.h"

UBGameplayAbility::UBGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

UAnimInstance* UBGameplayAbility::GetOwnerAnimInstance() const
{
	USkeletalMeshComponent* OwnerSkeletalMeshComponent = GetOwningComponentFromActorInfo();
	if (!OwnerSkeletalMeshComponent)
		return nullptr;

	return OwnerSkeletalMeshComponent->GetAnimInstance();
}
