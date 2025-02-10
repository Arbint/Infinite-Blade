// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA_Combo.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameplayTagsManager.h"

void UGA_Combo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!K2_CommitAbility())
	{
		K2_EndAbility();
		return;
	}

	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		UAbilityTask_PlayMontageAndWait* PlayComboMontageTask = 
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, ComboMontage);

		PlayComboMontageTask->OnBlendOut.AddDynamic(this, &UGA_Combo::K2_EndAbility);
		PlayComboMontageTask->OnInterrupted.AddDynamic(this, &UGA_Combo::K2_EndAbility);
		PlayComboMontageTask->OnCancelled.AddDynamic(this, &UGA_Combo::K2_EndAbility);
		PlayComboMontageTask->OnCompleted.AddDynamic(this, &UGA_Combo::K2_EndAbility);
		PlayComboMontageTask->ReadyForActivation();

		UAbilityTask_WaitGameplayEvent* WaitComboChangeEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,
			FGameplayTag::RequestGameplayTag("ability.combo.change"), nullptr, false, false);

		WaitComboChangeEvent->EventReceived.AddDynamic(this, &UGA_Combo::HandleComboChange);
		WaitComboChangeEvent->ReadyForActivation();
	}
	NextComboName = NAME_None;
}

void UGA_Combo::HandleComboChange(FGameplayEventData Payload)
{
	FGameplayTag EventTag = Payload.EventTag;
	
	if (EventTag == FGameplayTag::RequestGameplayTag("ability.combo.change.end"))
	{
		NextComboName = NAME_None;
		UE_LOG(LogTemp, Warning, TEXT("The next combo is cleared"))
		return;
	}

	TArray<FName> EventTagNames;
	UGameplayTagsManager::Get().SplitGameplayTagFName(EventTag, EventTagNames);
	NextComboName = EventTagNames.Last();
	UE_LOG(LogTemp, Warning, TEXT("The next combo is %s"), *(NextComboName.ToString()))
}
