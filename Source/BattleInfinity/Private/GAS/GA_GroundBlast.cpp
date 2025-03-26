// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA_GroundBlast.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GAS/BAbilitySystemStatics.h"
#include "GAS/TA_GroundPick.h"

UGA_GroundBlast::UGA_GroundBlast()
{
	ActivationOwnedTags.AddTag(UBAbilitySystemStatics::GetAimingStatTag());
	BlockAbilitiesWithTag.AddTag(UBAbilitySystemStatics::GetBasicAttackAbilityTag());
}

void UGA_GroundBlast::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
		return;

	UAbilityTask_PlayMontageAndWait* PlayCastMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,
		NAME_None, CastMontage);

	PlayCastMontage->OnBlendOut.AddDynamic(this, &UGA_GroundBlast::K2_EndAbility);
	PlayCastMontage->OnCancelled.AddDynamic(this, &UGA_GroundBlast::K2_EndAbility);
	PlayCastMontage->OnInterrupted.AddDynamic(this, &UGA_GroundBlast::K2_EndAbility);
	PlayCastMontage->OnCompleted.AddDynamic(this, &UGA_GroundBlast::K2_EndAbility);

	PlayCastMontage->ReadyForActivation();

	UAbilityTask_WaitTargetData* WaitTargetData = UAbilityTask_WaitTargetData::WaitTargetData(this,
		NAME_None, EGameplayTargetingConfirmation::UserConfirmed, GroundPickTargetActorClass);
	WaitTargetData->ValidData.AddDynamic(this, &UGA_GroundBlast::TargetReceived);
	WaitTargetData->Cancelled.AddDynamic(this, &UGA_GroundBlast::TargetCancelled);
	WaitTargetData->ReadyForActivation();

	AGameplayAbilityTargetActor* TargetActor = nullptr;
	WaitTargetData->BeginSpawningActor(this, GroundPickTargetActorClass, TargetActor);

	ATA_GroundPick* GroundPickTargetActor = Cast<ATA_GroundPick>(TargetActor);
	if (GroundPickTargetActor)
	{
		GroundPickTargetActor->SetTargetingDistance(TargetingMaxDistance);
		GroundPickTargetActor->SetTargetingAreaRadius(TargetingAreaRadius);
	}

	WaitTargetData->FinishSpawningActor(this, TargetActor);
}

void UGA_GroundBlast::TargetReceived(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (!K2_CommitAbility())
	{
		K2_EndAbility();
		return;
	}

	BP_ApplyGameplayEffectToTarget(TargetDataHandle, DamageEffect, GetAbilityLevel(CurrentSpecHandle, CurrentActorInfo));

	FHitResult CenterHitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 1);
	
	FGameplayCueParameters CueParams;
	CueParams.Location = CenterHitResult.ImpactPoint;
	CueParams.RawMagnitude = TargetingAreaRadius;
	GetAbilitySystemComponentFromActorInfo()->ExecuteGameplayCue(BlastVFXGameplayCueTag, CueParams);

	K2_EndAbility();
}

void UGA_GroundBlast::TargetCancelled(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	UE_LOG(LogTemp, Warning, TEXT("Target Cancelled"))
	K2_EndAbility();
}
