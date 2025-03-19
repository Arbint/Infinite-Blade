// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA_GroundBlast.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GAS/BAbilitySystemStatics.h"

UGA_GroundBlast::UGA_GroundBlast()
{
	ActivationOwnedTags.AddTag(UBAbilitySystemStatics::GetAimingStatTag());
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
}
